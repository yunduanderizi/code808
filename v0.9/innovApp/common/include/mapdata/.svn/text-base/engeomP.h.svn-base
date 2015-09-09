#ifndef ENGEOMP_H
#define ENGEOMP_H

#include <innovEngine_math.h>
#include <innovEngine_types.h>
#include <platform.h>

#include <graphics/grcolour.h>
#include <mapdata/enmfileP.h>
#include <mapdata/enpolyP.h>
#include <memory/allocator.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TGeomBBoxFormat {
  GEOM_BBOX_FORMAT_NONE,
  GEOM_BBOX_FORMAT_4BIT,
  GEOM_BBOX_FORMAT_8BIT,
  GEOM_BBOX_FORMAT_12BIT
} TGeomBBoxFormat;

typedef enum {
  GEOM_MAIN_CLASS_CITY_RAIL,
  GEOM_MAIN_CLASS_CITY_BUILDING,
  GEOM_MAIN_CLASS_CITY_TOWN,
  GEOM_MAIN_CLASS_POLY,
  GEOM_MAIN_CLASS_ARC
} TGeomMainClass;

typedef enum TGeomType {
  GEOM_TYPE_ARC,
  GEOM_TYPE_POLY,
  GEOM_TYPE_3D
} TGeomType;

typedef struct TGeomPos3D {
  /* Positional Data in cm relative to the 
   * geographical reference point of the 
   * object */
  INT16 x;
  INT16 y;
  INT16 z;
} TGeomPos3D;

typedef struct TGeomTexCord {
 /* (s,t) define the texture coordinate 
  * relative to the current texture width/height */
  INT16 s;
  INT16 t;
} TGeomTexCord;

/* The geometry context is used to provide additional information
 * required by the geometry DAL to extract binary data */
typedef struct TGeomContext {
  INT32            sectorX;     
  INT32            sectorY;
  UINT8            sectorRes;
  TGenericMapInfo *mapInfo;
  TAllocatorHandle allocator;
} TGeomContext;

typedef enum TGeomVertexFormat {
  GEOM_VERTEX_FORMAT_4BIT_REL,
  GEOM_VERTEX_FORMAT_8BIT_REL,
  GEOM_VERTEX_FORMAT_12BIT_REL,
  GEOM_VERTEX_FORMAT_12BIT_ABS
} TGeomVertexFormat;

typedef struct TGeomHeader{
  TGeomType      type;
  TGeomMainClass mainClass;
  UINT8          subClass;
  BOOL8          hasBBox;
  TRect          boundingBox;
  UINT16         attrib;     /* Context dependant Attribute data */
  UINT16         size;       /* Size (in bytes) of the geometry object in the map */
  UINT8         *addr;       /* Physical address of the data */
} TGeomHeader;

typedef struct TGeom {
  TGeomHeader     header;
  INT16           vertexCount;
  TPos            vertexList[2048]; //FIXME use #define

  /* Data used only by polygon objects */
  BOOL8  isFirstFake; /* TRUE if the first line segment is artificial */ 
  BOOL8  hasDonut;
  INT16  donutCount;
  INT16  donutStarts[128];
  INT16  segmentCount;
  INT16  segmentStarts[128];

} TGeom;

typedef struct TGeomMaterial {
  /* Material Colours */
  RGBCOLOUR  diffuseColour;     
  INT32      textureId;         /* -1 if no texture is present                      */
  BOOL8      textureRes;        /* Resolution shift to apply to texture coordinates */
  UINT16     polyCount;         /* Number of polygons having this material          */
  UINT16     *polyStarts;       /* Start index of each polygon                      */
} TGeomMaterial;

typedef struct TGeomPalette {
  UINT16     colourCount;
  RGBCOLOUR *colours;
} TGeomPalette;

typedef struct TGeomTexture {
  INT16    textureId;  /* Texture ID unique to the map          */
  INT16    paletteId;  /* Colour palette used by this texture   */
  UINT16   width;      /* Width of the texture, in pixels       */
  UINT16   height;     /* Height of the texture in pixels       */
  UINT8   *dataBlob;   /* Consists of 8 bits per channel RGB 
                          values for each of the pixels         */
} TGeomTexture;

typedef struct TGeom3DShape {
  UINT16         vertexCount;    /* Vertex Count                                 */  
  UINT16         texCoordCount;  /* Texture Coordinate Count                     */
  UINT16         matCount;       /* Material Count                               */
  UINT16         polyIndexCount; /* Number of rows in the polygon index array    */

  UINT8          res;            /* All the vertices need to be left shifted by this amount */
  TGeomPos3D    *vertices;       /* List of vertices for all polygons                       */
  TGeomTexCord  *texCoords;      /* List of texture coordinates for all polygons            */
  TGeomMaterial *materials;      /* List of materials, including polygons                   */
  UINT16        *polyIndex;      /* Array of polygon vertex/texture indexes                 */

} TGeom3DShape;

/* Geometry object representing a 3D landmark or any 
 * other 3D object */
typedef struct TGeom3D {
  TGeomHeader   header;    /* Contains a bounding rectangle and height data 
                              ( can be used to construct a bounding box ) */
  UINT16        geomId;    /* Unique geometry ID for this map */
  TPos          geoRef;    /* Geographical reference of this object. All vertices 
                              are stored relative to this position */
  INT16         numShapes; /* Number of shapes (polygons) that make up this object */
  TGeom3DShape *shapes;

} TGeom3D;


BOOL8 geom_unsafeUnpack3DBounds( 
    TGeom3D            *geom3D,
    const TGeom        *geom,
    const TGeomContext *geomContext ); 
/* Unpack a 3D object */
BOOL8 geom_unsafeUnpack3D( 
    TGeom3D            *geom3D,
    const TGeom        *geom,
    const TGeomContext *geomContext );

/* Destroy memory allocated to a 3D Geometry object */
BOOL8 geom_destroy3D( 
    TGeom3D            *geom3D, 
    const TGeomContext *geomContext );

INT32 geom_unsafeUnpackGeomHeader( 
    TGeomHeader        *geomHeader,
    const TGeomHeader  *lastGeomHeader,
    UINT8              *addr,
    const TGeomContext *geomContext );

UINT32 geom_unsafeUnpackPolygonDonuts( 
    TGeom              *geom,
    const TGeomContext *geomContext );

BOOL8 geom_unsafeUnpackGeomShell( 
    TGeom              *geom, 
    const TGeomContext *geomContext,
    BOOL8               unpackBBox );

BOOL8 geom_initGeomContext(TGeomContext     *geomContext,
                           TGenericMapInfo  *const mapInfo, 
                           TAllocatorHandle  allocator,
                           INT32             sectorX,
                           INT32             sectorY, 
                           INT32             sectorRes);

BOOL8 geom_unpackTexture( 
    TGeomTexture       *geomTexture, 
    UINT32              textureId,
    const TGeomContext *geomContext );

BOOL8 geom_destroyTexture( 
    TGeomTexture       *geomTexture, 
    const TGeomContext *geomContext );

BOOL8 geom_unpackPalette( 
    TGeomPalette       *geomPalette,
    UINT32              paletteId,
    const TGeomContext *geomContext );

BOOL8 geom_destroyPalette( 
    TGeomPalette       *geomPalette,
    const TGeomContext *geomContext );
#ifdef __cplusplus
}
#endif

#endif /* __ENGEOMP_H__ */
