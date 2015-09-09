#ifndef GRFONTCOLLECTION_H
#define GRFONTCOLLECTION_H

#include <graphics/grfontobj.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Graphics
/// @{
  
/// @addtogroup FontCollection
/// An object that queries a font palette based on various criteria.
/// @ingroup Graphics
/// @{

/** @typedef enum GETFONTPARAMS */
typedef enum GETFONTPARAMS
{
  /** */
  GETFONTPARAMS_NAME = 1,
  GETFONTPARAMS_SIZE = 2,
  GETFONTPARAMS_WEIGHT = 4,
  GETFONTPARAMS_STYLE = 8
} GETFONTPARAMS;

/** @typedef struct GETFONTSTRUCT */
typedef struct GETFONTSTRUCT
{
  GETFONTPARAMS gfp;
  const TFont *font;
  INT32 index;
} GETFONTSTRUCT;

/** @typedef struct TFontCollection 
    Represents a font collection.  All fields are private. Do not use. */
typedef struct TFontCollection

{
  /** @internal An array of pointers to fonts. */
  const TFont **fonts;
  /** @internal A count of how many fonts are present in the collection. */
  UINT32 fontCount;
} TFontCollection;

extern MAPCORE_API
void FontCollection_Define(TFontCollection *pThis, const TFont **fonts);

extern MAPCORE_API
const TFont *FontCollection_FindFont(TFontCollection *pThis,
                                     const char *name,
                                     UINT32 size,
                                     BOOL8 bold,
                                     BOOL8 italic);

extern MAPCORE_API
BOOL8 FontCollection_GetFont(TFontCollection *pThis, GETFONTSTRUCT *gfs);

extern MAPCORE_API
BOOL8 FontCollection_HasFont(TFontCollection *pThis, const TFont *font);

extern MAPCORE_API
const TFont *FontCollection_GetFontByIndex(TFontCollection *pThis, UINT32 index);

/// @}
/// @}

#ifdef __cplusplus
}
#endif

#endif
