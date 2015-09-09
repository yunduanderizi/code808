#ifndef GRPLATFORM_H
#define GRPLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Platform
/// @ingroup Graphics
/// @{

struct TGraphContext;
struct TPlatformGraphics;

/**
 * @name Public Types and Constants
 * @{
 */

/**
 * @brief Platform Graphics commands
 *
 * These are the commands which can be passed to a TPlatformGraphicsFunction
 * callback.
 */
typedef enum TPlatformGraphicsCommand {
  cmdCreate,        ///< Create a graphics context. Parameter is an optional handle.
  cmdDestroy,       ///< Destroy a graphics context. Parameter is ignored.
  cmdCreateBitmap,  ///< Create a bitmap. 
                    ///< Parameter is an in/out TBitmap. A framebuffer will be 
                    ///< created based on the TBitmap's attributes, and the 
                    ///< TBitmap's data pointer will be updated.
  cmdDestroyBitmap, ///< Destroy a bitmap.
                    ///< Parameter is the TBitmap to be destroyed.
  cmdSetForeColour, ///< Set the foreground colour used for text and graphics.
                    ///< Parameter is an RGBCOLOUR struct (not a pointer).
  cmdSetBackColour, ///< Set the background colour used for fills.
                    ///< Parameter is an RGBCOLOUR struct (not a pointer).
  cmdSetPenStyle,   ///< Set the pen style attributes (e.g. thickness and pattern).
                    ///< Parameter is a pointer to a TPenStyle (as yet undefined)
  cmdSetFillStyle,  ///< Set the fill style attributes (e.g. pattern).
                    ///< Parameter is a pointer to a TFillStyle (as yet undefined)
  cmdSetFont,       ///< Set the font. Parameter is a pointer to a TFont struct.
  cmdSetClipRect,   ///< Set the clipping region.
                    ///< Parameter is a TRect specifying max and min INCLUDED coords.
  cmdSetTransform,  ///< Specify a coordinate transform.
                    ///< Parameter is TMatrix2D.
  cmdMoveTo,        ///< Move the current pen / cursor position.
                    ///< Parameter is the new TPos.
  cmdRecordTextRect,///< Set up text bounds recording mode. The next call to 
                    ///< cmdDrawText.
                    ///< Parameter is the new TPos.
  cmdGetTextHeight, ///< Get the height of the current font.
                    ///< Pass in a pointer to a UINT32 out parameter.
  cmdDrawText,      ///< Draw a string of text, using the current pen and cursor.
                    ///< Pass a UTF16 pointer to the text string.
  cmdDrawLine,      ///< Reserved.
  cmdDrawLineTo,    ///< Draw a line from the cursor to the specified point.
                    ///< Parameter is a TPos in screen coordinates.
  cmdDrawEllipse,   ///< Draw an ellipse with axes aligned to the x and y screen 
                    ///< axes, and which touches all sides of a specified bounding 
                    ///< box.
                    ///< Parameter is a pointer to the TRect bounding box.
  cmdDrawPolygon,   ///< Reserved.
  cmdDrawRectangle, ///< Reserved.
  cmdDrawBitmap,    ///< Draw a bitmap, with its top-left corner aligned with the 
                    ///< cursor position.
                    ///< Parameter points to a TBitmap.
} TPlatformGraphicsCommand;

/**
 * @brief Type for Platform Graphics callback function.
 *
 * This function is called by various graphics functions to allow the platform 
 * to override specific graphics functionality.
 * 
 * The function is passed a pointer to the TGraphContext, a command and a 
 * command parameter. The type of the parameter varies for different commands.
 */
typedef BOOL8 TPlatformGraphicsFunction(struct TGraphContext *pThis,
                                        TPlatformGraphicsCommand cmd,
                                        const void *param);

/// Structure used to register a TGraphContext to the Platform Graphics layer.
typedef struct TPlatformGraphics {
  TPlatformGraphicsFunction *func;
  void *internalData;
} TPlatformGraphics;

///
/// Register a Platform Graphics handler.
/// @param pThis A TGraphContext object, or NULL. If this parameter is NULL, the
/// the handler is installed as a default, and attached to each new 
/// TGraphContext that is created.
/// @param graphics A Platform Graphics handler, defining a non-NULL callback.
/// @return TRUE is successful, otherwise FALSE.
///
BOOL8 GraphContext_RegisterPlatformGraphics(struct TGraphContext *pThis,
                                            TPlatformGraphics *graphics);

/// This declaration is provided to simplify callback declaration, but the
/// application doesn't need to use it.
BOOL8 os_graphics(struct TGraphContext *pThis,
                  TPlatformGraphicsCommand cmd,
                  const void *param);

//@}

#ifdef __cplusplus
}
#endif

#endif /* GRPLATFORM_H */
