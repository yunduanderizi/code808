#ifndef GRCOLOUR_H
#define GRCOLOUR_H

#include <platform_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Colour
/// RGB colour space object.
/// @ingroup Graphics
/// @{

///
/// @typedef struct RGBCOLOUR
/// Represents a colour object
///
typedef struct RGBCOLOUR // this structure must not be cast to a long integer
{                        // as its size might go beyond 4 bytes
  /// The blue component of the colour
  UINT8 blue;
  /// The blue component of the colour
  UINT8 green;
  /// The blue component of the colour
  UINT8 red;
  /// The alpha component of the colour (0x00 = fully opaque, 0xff = fully transparent)
  UINT8 alpha;
} RGBCOLOUR;

static const RGBCOLOUR RGB_NULL = {0,0,0,0};

extern MAPCORE_API
RGBCOLOUR RGBA_Define(UINT8 red, UINT8 green, UINT8 blue, UINT8 alpha);

extern MAPCORE_API
RGBCOLOUR RGB_Define(UINT8 red, UINT8 green, UINT8 blue);

extern MAPCORE_API
void RGB_Extract(RGBCOLOUR pThis, UINT8 *red, UINT8 *green, UINT8 *blue);

// Should this be public???
extern MAPCORE_API
RGBCOLOUR RGB_FromInternal(UINT16 colour, UINT8 depth);

extern MAPCORE_API
UINT16 RGB_ToInternal(RGBCOLOUR This, UINT8 depth);

/// @typedef struct TRgb8
/// 8bpp RGBT2222 pixel format
typedef UINT8 TRgb8;

/// @typedef struct TRgb16
/// 16bpp RGB565 pixel format
typedef UINT16 TRgb16;

/// @typedef struct TRgbX
/// Union of TRgb8 and TRgb16, used throughout deprecated graphics API
typedef UINT16 TRgbX;

/** Define a 16bpp colour
    @param r red component
    @param g green component
    @param b blue component
    @return The 16bpp colour index */
#define /* TRgb16 */ rgb16_define(/* UINT8 */ r, \
                                  /* UINT8 */ g, \
                                  /* UINT8 */ b) \
/* For faster conversion, with low accuracy... */ \
  (TRgb16)( (((b) >> 3) <<  0) |   \
            (((g) >> 2) <<  5) |   \
            (((r) >> 3) << 11)     \
          )
#if 0
/* For slower conversion, with better accuracy... */ \
  (TRgb16)( (((((b) < 0xfc) ? (b) + 0x04 : (b)) >> 3) <<  0) |   \
            (((((g) < 0xfe) ? (g) + 0x02 : (g)) >> 2) <<  5) |   \
            (((((r) < 0xfc) ? (r) + 0x04 : (r)) >> 3) << 11)     \
          )
#endif

/** Dither an RGB triple to 16bpp, using an ordered dither
    @param r red component
    @param g green component
    @param b blue component
    @param x The pixels x coordinate
    @param y The pixels y coordinate
    @return The 16bpp colour index */
extern MAPCORE_API
TRgb16 rgb16_dither(UINT8 r, UINT8 g, UINT8 b, INT32 x, INT32 y);

/** Extract the red, green and blue components of a 16bpp colour
    @param colour A 16bpp colour
    @param [out] r red component output location
    @param [out] g green component output location
    @param [out] b blue component output location
    @note
 * 16 bit colour index is made as follows:
 *
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |   red   |   green   |   blue  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |          |          |  
 *      |          |          +-----> 5 bits: msb of blue  component
 *      |          +----------------> 6 bits: msb or green component
 *      +---------------------------> 5 bits: msb of red   component
 */ 
static INLINE void rgb16_extract(TRgb16 c, UINT8 *r, UINT8 *g, UINT8 *b)
{
//  For faster conversion, with low accuracy...
  *b = (UINT8)((c &         0x1F) << 3);
  *g = (UINT8)((c & (0x3F <<  5)) >> 3); 
  *r = (UINT8)((c & (0x1F << 11)) >> 8); 
//  For slower conversion, with better accuracy (required for good alpha blending)
//  *b = (UINT8)(((c &         0x1F) << 3) | ((c &    0x1F) >> 2 ));
//  *g = (UINT8)(((c & (0x3F <<  5)) >> 3) | ((c & (3<< 9)) >> 9 ));
//  *r = (UINT8)(((c & (0x1F << 11)) >> 8) | ((c & (7<<13)) >> 13));
}

/** Convert a 16bpp colour to 8bpp
    @param colour A 16bpp colour
    @return The 8bpp colour index */
extern MAPCORE_API
TRgb8 rgb16_toRgb8(TRgb16 colour16);

/** Convert a 16bpp colour to 8/16bpp, depending on the depth parameter
    @param colour A 16bpp colour
    @param depth The bit depth of the output colour
    @return The 8/16bpp colour index */
extern MAPCORE_API
TRgbX rgb16_toRgbX(TRgb16 colour, UINT32 depth);


/** Define an 8bpp colour
    @param r red component
    @param g green component
    @param b blue component
    @return The 8bpp colour index */
#define /* TRgb8 */ rgb8_define(/* UINT8 */ r, \
                                /* UINT8 */ g, \
                                /* UINT8 */ b) \
  (TRgb8)(   ((((r) & 0xC0) >> 4) |    \
              (((g) & 0xC0) >> 2) |    \
               ((b) & 0xC0)            \
             ) |                       \
           ( ( ((r) & 0x3F) +          \
              (((g) & 0x3F) << 1) +    \
               ((b) & 0x3F)            \
             ) >> 6                    \
           )                           \
         )                           
  
/** Extract the red, green and blue components of an 8bpp colour
    @param colour An 8bpp colour
    @param [out] r red component output location
    @param [out] g green component output location
    @param [out] b blue component output location */
extern MAPCORE_API
void rgb8_extract(TRgb8 c, UINT8 *r, UINT8 *g, UINT8 *b);

/** Convert an 8bpp colour to 16bpp
    @param colour An 8bpp colour
    @return The 16bpp colour index */
extern MAPCORE_API
TRgb16 rgb8_toRgb16(TRgb8 colour8);

/** Convert an 8bpp colour to 8/16bpp, depending on the depth parameter
    @param colour An 8bpp colour
    @param depth The bit depth of the output colour
    @return The 8/16bpp colour index */
extern MAPCORE_API
TRgbX rgb8_toRgbX(TRgb8 colour, UINT32 depth);

/** Define an 8/16bpp colour, depending on the depth parameter
    @param depth The bit depth of the colour parameter
    @param r red component
    @param g green component
    @param b blue component
    @return The 8/16bpp colour index */
extern MAPCORE_API
TRgbX rgbX_define(UINT32 depth, UINT8 r, UINT8 g, UINT8 b);

/** Extract the red, green and blue components of an 8/16bpp colour
    @param colour An 8/16bpp colour
    @param depth The bit depth of the colour parameter
    @param [out] r red component output location
    @param [out] g green component output location
    @param [out] b blue component output location */
extern MAPCORE_API
void rgbX_extract(TRgbX colour, UINT32 depth, UINT8 *r, UINT8 *g, UINT8 *b);

/** Convert an 8/16bpp colour to 8bpp, depending on the depth parameter
    @param colour An 8/16bpp colour
    @param depth The bit depth of the colour parameter
    @return The 8bpp colour index */
extern MAPCORE_API
TRgb8 rgbX_toRgb8(TRgbX colour, UINT32 depth);

/** Convert an 8/16bpp colour to 16bpp, depending on the depth parameter
    @param colour An 8/16bpp colour
    @param depth The bit depth of the colour parameter
    @return The 16bpp colour index */
extern MAPCORE_API
TRgb16 rgbX_toRgb16(TRgbX colour, UINT32 depth);

/// @}

#ifdef __cplusplus
}
#endif

#endif // GRCOLOUR_H
