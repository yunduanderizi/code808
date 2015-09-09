#ifndef GRBITMAP_H
#define GRBITMAP_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Bitmap
/// Create a bitmap object and load bitmap data from persistent storage.
/// @ingroup Graphics
/// @{

/** @typedef enum PIXELFORMAT */
typedef enum PIXELFORMAT
/** 8 or 16 bit colour pixels. */
{
  /** 8 bit (palette based) pixel. */
  PIXELFORMAT_RGB323,
  /** 16 bit packed pixel. */
  PIXELFORMAT_RGB565,
  /** 32 bit packed pixel. */
  PIXELFORMAT_RGBA8888
} PIXELFORMAT;

/** @typedef struct TBitmap */
typedef struct TBitmap
/** Bitmap structures are use for fonts and POI icons among other things. */
{
  /** Previous bitmap in linked list */
  struct TBitmap *previous;
  /** Next bitmap in linked list */
  struct TBitmap *next;
  /** Width (in pixels) */
  UINT16 width;
  /** Height */
  UINT16 height;
  /** Depth (number of bits per pixels) */
  UINT8 depth;
  /** Used when searching for bitmaps */
  char name[16];
  /** Pointer to the bitmap pixel data */
  UINT8 *data;
  /** If TRUE, pixels of colour 0x00 are not drawn */
  BOOL8 transparentFlag;
  /** If true then the pixel data is LZW compressed */
  BOOL8 compressedFlag;
  /** If storageFlag is TRUE then data points to a TLiteBitmap 
      describing the file that the bitmap is stored in.
      
      @todo
      EVENTUALLY WE NEED TO DEFINE A CLEANER MECHANISM */
  BOOL8 storageFlag;
  /** Is the bitmap based upon 8 or 16 bit pixels? */
  PIXELFORMAT pixelFormat;
} TBitmap;

/** lower 4 bits of TFileBitmap.format for the 2 colour bitmap format */
#define FORMAT_1BPP 0
/** lower 4 bits of TFileBitmap.format for the 4 colour bitmap format */
#define FORMAT_2BPP 1
/** lower 4 bits of TFileBitmap.format for the 16 colour bitmap format */
#define FORMAT_4BPP 2
/** lower 4 bits of TFileBitmap.format for the 256 colour bitmap format */
#define FORMAT_8BPP 3
/** lower 4 bits of TFileBitmap.format for the 16bpp colour bitmap format */
#define FORMAT_16BPP 4
/** lower 4 bits of TFileBitmap.format for the 24bpp colour bitmap format */
#define FORMAT_24BPP 5
/** lower 4 bits of TFileBitmap.format for the 32bpp RGBA bitmap format */
#define FORMAT_32BPP 6

/** Upper 4 bits of TFileBitmap.format when the bitmap data is 
    uncompressed */
#define FORMAT_UNCOMPRESSED (0 << 4)
/** Upper 4 bits of TFileBitmap.format when the bitmap data is LZW 
    compressed */
#define FORMAT_LZWCOMPRESSED (1 << 4)
/** Upper 4 bits of TFileBitmap.format when the bitmap data is stored in 
    a file.
    
    @todo NOT CURRENTLY SUPPORTED BUT THIS MECHANISM IS SUGGESTED AS A WAY 
    OF FLAGGING ON DISC BITMAPS IN FUTURE */
#define FORMAT_STORED (2 << 4)

/** bit position in TFileBitmap.flags for the transparency flag 
    representing that logical colour 0 of the bitmap should be treated as 
    being transparent. */
#define FLAG_TRANSPARENT (1 << 0)

/** 'Lite' bitmaps are stored in the filing system and are loaded
    whenever they are displayed.  This structure maintains information that
    enables 'Lite' bitmaps to be read.
    
    @todo
    THE CURRENT MECHANISM FOR HANDLING STORED BITMAPS SHOULD BE 
    REWORKED SLIGHTLY. */
typedef struct TLiteBitmap {
  /** data size of bitmap in file */
  UINT32 dataSize;
  /** file offset */
  UINT32 offset;
  /** bitmap file name */
  char fname[128];
} TLiteBitmap;

/*----------------------------------------------------------------*/

/** This is the header that is defined for bitmaps that are stored on disc.
    
    @par
    EVENTUALLY BITMAPS THAT ARE STORED IN MEMORY SHOULD BE ALTERED
    TO USE THIS STRUCTURE ALSO. */
typedef struct TFileBitmap {
  /** total size of bitmap bytes including the header */
  UINT32 totalSize;
  /** width of bitmap */
  UINT16 width;
  /** height of bitmap */
  UINT16 height;
  /** bitmap format (i.e. number of bits per pixel and compressed flag) */
  UINT8 format;
  /** a number of flags representing various bitmap properties like 
  transparency */
  UINT8 flags;
  /** bitmap name */
  char name[74];
} TFileBitmap;

extern MAPCORE_API
void Bitmap_Create(TBitmap *pThis,
                   UINT32 width,
                   UINT32 height,
                   UINT32 stride,
                   UINT8 *frameBuffer,
                   PIXELFORMAT pixelFormat);

/** Loads a Windows BMP file.

    @param filename The name of the .bmp file containing the bitmap.

    @param buffer Points to a 32 BIT ALIGNED user supplied 
    workspace that is used to contain bitmap data.  The workspace 
    should be large enough to accommodate both the header of the 
    bitmap (TBitmap) as well as the actual bitmap data. If NULL, a 
    workspace will be allocated in the bitmap cache (of size = 
    workspaceSize).

    @param workspaceSize Represents the total size of the available 
    workspace.  Typically (width*height*(bpp/8)) + sizeof(TBitmap).

    @return NULL on failure, otherwise a pointer to the loaded bitmap. */
extern MAPCORE_API
TBitmap *Bitmap_LoadBmpFile(const char *filename, UINT8 *buffer, UINT32 bufSize);

/** Loads a bitmap from a file.

    @param filename The name of the .bm file containing the bitmap.

    @param bitmapName Points to the name of the bitmap being sought.

    @param workspace Points to a 32 BIT ALIGNED user supplied 
    workspace that is used to contain bitmap data.  The workspace 
    should be large enough to accommodate both the header of the 
    bitmap (TBitmap) as well as the actual bitmap data. If NULL, a 
    workspace will be allocated in the bitmap cache (of size = 
    workspaceSize).

    @param workspaceSize Represents the total size of the available 
    workspace.  Typically (width*height*(bpp/8)) + sizeof(TBitmap).

    @return NULL on failure, otherwise a pointer to the loaded bitmap. */
extern MAPCORE_API
TBitmap *Bitmap_Load(const char *filename,
                     const char *bitmapName,
                     UINT8 *workspace,
                     UINT32 workspaceSize);


/** Set the compressed bitmap decompression buffer.

    @param buffer This buffer is used to store the bitmap scanline data
    when it is decompressed.  It should be large enough to contain the
    uncompressed pixel data for the largest bitmap used (typically the
    size of the screen).

    @param size The size of the decompression buffer (typically screen 
    width * screen height).

    @todo WE SHOULD REVIEW MEMORY USE BY THE GRAPHICS SUBSYSTEM TO TRY AND 
    REDUCE THE NUMBER OF BUFFERS USED. */
extern MAPCORE_API
void graph_setDecompBuffer(UINT8 *buffer, UINT32 size);
  
/** Sets up a cache that is used by the bitmap routines for caching
    stored bitmaps to decrease decompression and loading time.

    @param buffer Points to an area of memory to use as a bitmap cache.  
    The size of this memory should be large enough to hold several 
    screenfuls of video data (e.g. around 128 - 256kB for a 
    320 x 240 screen).

    @param size The size in bytes of the bitmap cache.

    @todo WE SHOULD REVIEW MEMORY USE BY THE GRAPHICS SUBSYSTEM TO 
    TRY AND REDUCE THE NUMBER OF BUFFERS USED. */
extern MAPCORE_API
void graph_setCacheBuffer(UINT8 *buffer, UINT32 size);

/// @} end of group


// internal call add by Edison at 2008.9.24
extern TBitmap *Bitmap_LoadEx(const char *filename, int iStyle, 
                              const char *bitmapName,
                              UINT8      *workspace,
                              UINT32      workspaceSize);
//@CHN
#define POI_GLOBAL_BLOCK 256*1024
#ifdef __cplusplus
}
#endif

#endif
