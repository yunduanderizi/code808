/*
 * This file contains the generic 3D display engine API.
 * 
 * It does not make any assumption on layers (road, poi, polygons,
 * arcs...) that are drawn in the 3D display.
 *
 * Author: Dominique Pelle <dpelle@21com.com>
 */
#ifndef DI3DENGI_H
#define DI3DENGI_H

#include <innovEngine_types.h>
#include <display/dipolstyle.h>
#include <graphics/grcontext.h>
#include <graphics/grcolour.h>

#ifdef __cplusplus
extern "C" {
#endif

/* BEGIN HACK */
struct TViewport;
/* END HACK */

#ifndef PI
#define PI 3.14159265358979323846
#endif

/** Minimum and maximum 3D view vertical angles in degrees that
    should be passed to disp3d_setVerticalAngle() */
#define DISP3D_MIN_VA  0
#define DISP3D_MAX_VA 65
  
/** Minimum and maximum 3D view observation distance in metres
    that should be passed to disp3d_setObservationDistance() */
#define DISP3D_MIN_DIST 100
#define DISP3D_MAX_DIST 20000

/*
 * Maximum number of vertices (after clipping) of a 3D polygon (polygons
 * with more vertices will be discarded with warning being logged).
 * 3D polygon have less vertices than 2D polygons.
 */
#define DISP3D_MAX_CLIPPED_3D_VERTEX 255

/* Plane equation a.x + b.y + c.z + d = 0 */
typedef struct TPlane_d {
  FLOAT64 a, b, c, d;
} TPlane_d;

/* Plane equation in 64 bits fixed point arithmetics */
typedef struct TPlane_i {
  INT64 a, b, c, d;
} TPlane_i64;

/* Plane equation in 32 bits fixed point arithmetics */
typedef struct TPlane_i32 {
  INT32 a, b, c, d;
} TPlane_i32;

/*
 * A rectangular 3D box.
 *
 * The first fields of TBox are ordered in such a way that
 * we can cast a TBox into a TRect when that makes sense
 */
typedef struct TBox {
  INT32 xmin, ymin, xmax, ymax;   /* same fields as TRect */
  INT32 zmin, zmax;
} TBox;

/*
 * Fixed point arithmetic bit shifts carefully chosen so that
 * there are no overflows and precision is good enough. The
 * bigger shifts are, the better the precision but the more
 * risky it is to introduce overflows.  Assertions in the
 * code check for possible overflows.
 */
#define DISP3D_SHIFT_ROT32        10
#define DISP3D_SHIFT_ROT64        29
#define DISP3D_SHIFT_IROT32       10
#define DISP3D_SHIFT_PL64         28
#define DISP3D_SHIFT_PL32         10
#define DISP3D_SHIFT_VERT_PL      11
#define DISP3D_SHIFT_DIST1         7
#define DISP3D_SHIFT_DIST2        14
#define DISP3D_SHIFT_DEPTH_FACTOR  7
#define DISP3D_SHIFT_V_I          15

/*
 * Macro used to check that fixed point arithmetics do
 * not overflow
 */
#define DISP3D_MAX_FIXED(bitCount) (0xFFFFFFFFUL >> (32 - (bitCount)))

/* Round a floating point value and transform it into an integer value */

/*
 * Macro to apply depthFactor to depth
 *
 * depthFactor is a coefficient between [0..1] stored in
 * fixed point format.  The depthFactor of a given point
 * on the map is the cosine of the vertical observation
 * angle.  In other words, depthFactor is 1 when point is
 * seen vertically and close to zero for far points near
 * the horizon.  Objects rendering depends on depth as
 * well as depthFactor.  For example, a road at a given
 * depth should be rendered with more details if its
 * depthFactor is 1 than if depthFactor is small.
 */
#define DEPTH(depth, depthFactor) \
  (((depth*depthFactor) >> DISP3D_SHIFT_DEPTH_FACTOR))

/*
 * Pyramid of vision clipping plane.  Pyramid of vision is
 * limited by 6 planes.
 *
 * 3D buildings don't need to be rendered when they are far
 * away so buildings use a back plane which is closer than
 * the back plane used by any other object.
 */
typedef enum TPlaneIdx {
  DISP3D_PL_LEFT = 0,
  DISP3D_PL_RIGHT,
  DISP3D_PL_DOWN,
  DISP3D_PL_UP,
  DISP3D_PL_FRONT,
  DISP3D_PL_BACK,          /* clipping back plane for anything else */
  DISP3D_PL_BACK_BUILDING, /* clipping back plane for buildings */
  DISP3D_PL_FRONT_BUILDING, /* clipping front plane for non buildings */
  DISP3D_PL_COUNT,
  DISP3D_PL_NONE
} TPlaneIdx;

/*
 * Don't display any building polygon it it is further than ?? m
 * Be careful before increasing this value too high as 3D painter
 * algorithm buffer would need to be increased as well 
 */
#define BUILDING_DIST_THRESHOLD 600

typedef struct TPos3d {
  INT32 x, y, z;
} TPos3d;

/* 
 * Maximum number of vertices before clipping (+1
 * to close polygons)
 */
#define DISP3D_POLY_MAX_UNCLIPPED_VERT (1024 + 1)

/*
 * Max nb of vertices after clipping
 * Clipping is done against a maximum of 4 planes.
 * It can potentially multiply the number of vertices
 * by 4 (*very* pessimistic) However, this is too pessimistic
 * and in practice impossible.  So we set the maximum number
 * of vertices after clipping to the number of vertices
 * before clipping plus a small margin.  If for some weird
 * reasons, clipping results in more points than 
 * DISP3D_POLY_MAX_CLIPPED_VERT, code would not cause
 * any corruption but clipping would discard the polygon.
 * Once again, this is not possible in practise.  In fact,
 * clipping of large polygon generally reduces the number
 * of vertices rather than increase it in most cases because
 * several contiguous vertices are not visible.
 */
#define DISP3D_POLY_MAX_CLIPPED_VERT (1024 + 20)

/* Flag controlling polygon rendering */
#define DISP3D_POL_CONVEX  1  /* convex flag polygon (faster to render) */
#define DISP3D_POL_AA      2  /* antialiased flag */

/* Polygon 2D */
typedef struct TPol2d {
  TPolStyle style;                     /* solid or pattern polygon */
  INT16     n;                         /* Number of shell vertices */
  INT16     holeCount;                 /* Number of donut holes */
  INT32     flag;                      /* convex?  antialiased? */

  /* 
   * Indices of donut hole start.  First hole should start just after shell
   * vertices (so donutStart[0] should be equal to n).  donutStart[holeCount]
   * should point just after the last hole and holes vertices should be 
   * contiguous so that it is possible to compute number of vertices in
   * hole i with:  donutStart[i + 1] - donutStart[i].
   */
  INT16 *donutStart;

  /* Array of vertices holding polygon shell and donut holes if any */
  TPos  *v;
} TPol2d;


/* TODO: TPol3d should use TPolStyle */
typedef struct TPol3d {
  TPolKind kind;  /* kind of polygon: solid, pattern or textured */
  INT16    n;     /* number of vertices */
  TPos3d  *v;     /* array of vertex positions */
  
  union {
    struct {
      /* Fields for solid polygons */
      RGBCOLOUR fillColour, borderColour;
    } s;
    struct {
      /* Fields for textured polygons */
      TPos     *tv;
      TTexture *texture;
    } t;
  } u;
} TPol3d;

/*
 * Camera model type.
 *
 * This structure needs to be updated whenever the camera model
 * changes (i.e. observation angle or observation distance is
 * updated or when vehicle moves).  disp3d_refreshCamera() updates
 * the structure.
 */
typedef struct TCamera {
  /* Parameters that can be set by user */
  INT32   dist;     /* distance of observation (integer) */
  FLOAT64 va;       /* angle of observation vs vertical (rad) */
  FLOAT64 wa2;      /* half width camera aperture angle */
  INT32   lat, lon; /* GPS position of vehicle */
  FLOAT64 heading;

  INT32 xmin_s, xmax_s, ymin_s, ymax_s; /* viewport in screen coord */
  INT32 viewportWidth, viewportHeight;  /* viewport size */
  INT32 screenOffsetX, screenOffsetY;   /* offset of center of the
                                         * viewport from screen origin */

  /* Following parameters are computed... */

  /*
   * needRefresh is true when camera parameters are not up to date
   * and need to be recalculated
   */
  BOOL8 needRefresh;

  /* Camera position in world coordinates */
  INT32 x_w, y_w, z_w;

  /*
   * Fixed point value of cos(latitude) and 1/cos(latitude)
   * used to convert to/from metre from/to centered coordinates.
   * X coordinates in centered coordinate have to be multiplied
   * by cosLat to get x value in metres (nothing to do for Y
   * coordinates).  
   *
   * If we don't convert from centered coordinates to metre,
   * map display would be wrong in regions near the poles. 
   * It is good to test display in northern regions such as
   * Oslo (Sweden) for example to ensure that cosLat is well
   * taken into account.
   */
  INT32 cosLat;      /* 8 bits after decimal dot */
  INT32 invCosLat;   /* 7 bits after decimal dot */
  
  /*
   * The visible area on the map is not a rectangle (it is
   * a trapezoid).  However, we compute a rectangle which
   * includes the entire visible area on the map to quickly
   * reject non visible objects.  This bounding box is for
   * objects on the ground only (z=0).
   */
  TRect bounds_w;
  TRect bounds_c;

   /*
    * Visible bounding box for objects that can have z > 0
    * Bounds3d_w should include bounds_w
    */
  TRect bounds3d_w;

  /*
   * Width and height angle determine the aperture of the pyramid
   * of vision.  Width angle can be configured, height angle is
   * computed from width angle and screen aspect ratio.
   */
  FLOAT64 ha2;

  /* Equation of planes of pyramid of vision in eye and world coordinates */
  TPlane_d   pl_de[DISP3D_PL_COUNT];
  TPlane_d   pl_dw[DISP3D_PL_COUNT];
  TPlane_i64 pl_iw[DISP3D_PL_COUNT];
  TPlane_i32 pl_ic[DISP3D_PL_COUNT];

  /*
   * Equation of vertical planes in centered coordinates around the
   * visible trapezoid 
   */
  TPlane_i32 vertPlane_ic[DISP3D_PL_COUNT];

  /*
   * Indexes required clipping planes.  Even though the pyramid of vision
   * is limited by 6 planes, a maximum of 4 planes needs to be checked
   * for clipping.  The 4 planes to select depend on the vertical angle.
   * For example, for a vertical view, it is useless to check for the
   * back plane since all points on the map will be in front of the
   * back plane. clipPlanes contains the indexes of the planes that
   * need to be used.
   */
# define MAX_CLIP_PLANES (4 + 1)
  TPlaneIdx clipPlanes[MAX_CLIP_PLANES];

  /* Visualisation plane (or projection plane depth) */
  FLOAT64 plane_v_d;
  INT32 plane_v_i, plane_shifted_v_i; /* same in integer */

  /* Depending on vertical angle, horizon may or may not be visible */
  BOOL8 isVisibleHorizon;
  FLOAT64 horizonAngle;

  /* Depth of front and back planes */
  INT32 z_min_e, z_max_e;

  /* 3D rotation matrix from centered to eye coordinates */
  FLOAT64 rot[3][3];
                        /* | centered on eye to to eye coord */
  /* Inverse matrix of rot[][] */
  FLOAT64 irot[3][3];

  /* Same as rot/irot but in 64 an 32 bits fixed point numbers */
  INT64  rot_i64[3][3];
  INT32  rot_i32[3][3];
  INT32  irot_i32[3][3];
  
  /* metric (mm) rotation matrix for orthogonal metric to view space transformations*/
  INT16 rot_metric[3][3];

  /* These values are often used so they are computed once for all */
  INT32 z_w_time_rot_i32_0_2;    /* z_w * rot_i32[0][2] */
/*INT32 z_w_time_rot_i32_1_2;*/  /* not used since rot[1][2] == 0 */
  INT32 z_w_time_rot_i32_2_2;
  INT32 c_time_z_w_plus_d[DISP3D_PL_COUNT];

  INT32  minDepth;
  INT32  middleDepth;
  INT32  maxDepthFactor;
} TCamera;

extern MAPCORE_API
void Camera_SetHalfWidthApertureAngle(TCamera *pThis, FLOAT64 wa2 /* in degrees */);

extern MAPCORE_API
void Camera_SetHeadingAngle(TCamera *pThis, INT32 heading /* in 1/10000 degrees from north */);

extern MAPCORE_API
void Camera_SetLatLon(TCamera *pThis, INT32 lat, INT32 lon);

extern MAPCORE_API
void Camera_SetObservationDistance(TCamera *pThis, INT32 dist);

extern MAPCORE_API
void Camera_SetVerticalAngle(TCamera *pThis, FLOAT64 va /* in degrees */);

extern MAPCORE_API
void Camera_SetViewport(TCamera *pThis,
                        INT32 xmin, INT32 ymin, INT32 xmax, INT32 ymax);

extern MAPCORE_API
UINT8 Camera_GetHeadingAngle(const TCamera *pThis);

extern MAPCORE_API
UINT32 Camera_GetObservationDistance(const TCamera *pThis);

extern MAPCORE_API
FLOAT64 Camera_GetVerticalAngle(const TCamera *pThis);

BOOL8 Camera_ClipLine2d_w(const TCamera *pThis,
                          TPos *pc1_w, TPos *pc2_w,
                          const TPos *p1_w, const TPos *p2_w);

#ifdef __cplusplus
}
#endif

#endif
