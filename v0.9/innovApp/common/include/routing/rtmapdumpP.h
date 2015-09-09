#ifndef RTMAPDUMPP_H
#define RTMAPDUMPP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <innovEngine_types.h>

/*----------------------------------------------------------------*/
/* TYPES */
/*----------------------------------------------------------------*/
/// These emulate some styles in shapefil.h
typedef enum {
  MAPDUMP_STYLE_NODES = 1,
  MAPDUMP_STYLE_LINES = 3
} TMapdumpStyle;


/// Type for holding a couple of shaplib file handles
typedef struct TMapdumpFile {
  void*         shpFile;
  void*         dbfFile;
  TMapdumpStyle style;
} TMapdumpFile;


/*----------------------------------------------------------------*/
/* INCLUDES - done here to prevent some circularities. */
/*----------------------------------------------------------------*/
#include <routing/rtsegmentlistP.h>
#include <routing/rtsearchnodeP.h>


/*----------------------------------------------------------------*/
/* FUNCTION DECLARATIONS */
/*----------------------------------------------------------------*/
// Set the global prefix appended to all filenames
void mapdump_setGlobalPrefix( 
  const char* prefix );


// Open a file for dumping search nodes
//
// @param pThis a TMapdumpFile object in which to store the file handles
// @param name The file name to use.  This should not include and
//   extension, and the global name prefix will be prepended.
// @param style MAPDUMP_STYLE_LINES or MAPDUMP_STYLE_NODES
// @return FALSE on error, TRUE otherwise
BOOL8 mapdumpFile_openSearchNodeFile( 
  TMapdumpFile* pThis, 
  const char* name, 
  TMapdumpStyle style );


// Dump a search node and lots of extra data in the specified style
//
// @param pThis A pair of mapdump handles created by
//    mapdumpFile_openSearchNodeFile()
// @param node The TSearchNode to dump
// @return FALSE on error, TRUE otherwise
BOOL8 mapdumpFile_dumpSearchNode( 
  TMapdumpFile* pThis,
  const TSearchNode* node );

/// Dump a search node (and any implicity connected nodes) and lots
/// of extra data in the specified style
///
/// @param pThis A pair of mapdump handles created by
///    mapdumpFile_openSearchNodeFile()
/// @param node The TSearchNode to dump
/// @param expand If expand is FALSE we skip the head of the search 
///   node and expand down any other intermediate segments
/// @return FALSE on error, TRUE otherwise
BOOL8 mapdumpFile_dumpSearchNodeList( 
  TMapdumpFile* pThis,
  TSearchNode* node,
  BOOL8 expand);



// Close a previously opened TMapdumpFile object
//
// @param pThis a TMapdumpFile object with open handles
void mapdumpFile_closeFile( 
  TMapdumpFile* pThis );


// Dump a segment list to a shapefile.
//
// @param pThis The segment list to dump
// @param name The file name to use.  This should not include and
//   extension, and the global name prefix will be prepended.
void segmentList_dumpShapeFile( 
  TSegmentList* pThis,
  const char* name );

#ifdef __cplusplus
}
#endif

#endif // RTMAPDUMPP_H
