#ifndef DI3DENGIP_H
#define DI3DENGIP_H

#include <display/di3Dengi.h>
#include <innovEngine_types.h>

struct TViewport;

#ifdef __cplusplus
extern "C" {
#endif

// BEGIN PRIVATE

/*
 * Maximum latitude for map origin (+ or -).
 * Map projection used in map core cannot project
 * correctly near polar regions so setting map
 * origin beyond 80 degree (and beyond -80 degrees)
 * is prevented.
 */
#define MAX_LATITUDE 8000000

void Camera_Centered2dToScreen2d(const TCamera *pThis,
                                 TPos *pos_s, const TPos *pos_c);
void Camera_Centered2dToScreen3d(const TCamera *pThis,
                                 TPos3d *pos_s, const TPos *pos_c);
void Camera_Centered3dToScreen2d(TCamera *pThis,
                                 TPos *pos_s, const TPos3d *pos_c);
void Camera_Centered3dToScreen3d(TCamera *pThis,
                                 TPos3d *pos_s, const TPos3d *pos_c);
BOOL8 Camera_ClipLine2d_c(const TCamera *pThis,
                          TPos *pc1_c, TPos *pc2_c,
                          const TPos *p1_c, const TPos *p2_c);
TPol2d *Camera_ClipPolygon2d_c(const TCamera *pThis,
                               TPol2d *pol_c,
                               const TPlaneIdx *clipPlanes);
BOOL8 Camera_ClipPolygon2d_w(const TCamera *pThis, TPos pos_w[], INT32 *n);
TPol3d *Camera_ClipPolygon3d_c(const TCamera *pThis,
                               TPol3d *pol_c,
                               const TPlaneIdx *clipPlanes);
void Camera_DepthFactorPoint2d_c(const TCamera *pThis,
                                 const TPos *p_c,
                                 INT32 *depth, INT32 *depthFactor);
void Camera_DepthFactorPoint2d_w(const TCamera *pThis,
                                 const TPos *p_w,
                                 INT32 *depth, INT32 *depthFactor);
void Camera_DepthFactorRect_c(TCamera *pThis,
                              const TRect *rect_c,
                              INT32 *depth, INT32 *depthFactor);
void Camera_DepthFactorRect_w(TCamera *pThis,
                              const TRect *rect_w,
                              INT32 *depth, INT32 *depthFactor);
INT32 Camera_DepthPoint2d_c(TCamera *pThis, const TPos *p_c);
INT32 Camera_DepthPoint2d_s(TCamera *pThis, const TPos *pos_s);
INT32 Camera_DepthPoint2d_w(TCamera *pThis, const TPos *p_w);
INT32 Camera_DistRect_w(TCamera *pThis, const TRect *rect_w);
BOOL8 Camera_DrawPol2d_c(struct TViewport *pThis,
                         TPol2d *pol_c,
                         const TPlaneIdx *clipPlanes);
BOOL8 Camera_DrawPol3d_c(TCamera *pThis,
                         TPol3d *pol_c,
                         const TPlaneIdx *clipPlanes, struct TViewport *viewport);
void Camera_Eye3dToWorld3d(TCamera *pThis,
                           TPos3d *pos_w, const TPos3d *pos_e);
TPlaneIdx *Camera_FindClipPlanesRect_c(TCamera *pThis,
                                       const TRect *bounds_c,
                                       TPlaneIdx clipPlanesBuffer[]);
TPlaneIdx *Camera_FindClipPlanesRect_w(TCamera *pThis,
                                       const TRect *bounds_w,
                                       TPlaneIdx clipPlanesBuffer[]);
const TRect *Camera_GetVisibleRect(const TCamera *pThis);
BOOL8 Camera_IsVisibleBox_w(const TCamera *pThis, const TBox *box_w);
BOOL8 Camera_IsVisibleRect_c(const TCamera *pThis, const TRect *bounds_c);
BOOL8 Camera_IsVisibleRect_w(const TCamera *pThis, const TRect *bounds_w);
void Camera_MapToScreen(TCamera *pThis,
                        INT32 *outx, INT32 *outy,
                        INT32 inx, INT32 iny);
BOOL8 Camera_QuickClipLine2d_c(const TCamera *pThis,
                               TPos *pc1_c, TPos *pc2_c,
                               const TPos *p1_c, const TPos *p2_c,
                               const TPlaneIdx *clipPlanes);
BOOL8 Camera_QuickClipLine2d_w(const TCamera *pThis,
                               TPos *pc1_w, TPos *pc2_w,
                               const TPos *p1_w, const TPos *p2_w,
                               const TPlaneIdx *clipPlanes);
BOOL8 Camera_QuickIsVisibleLine2d_c(const TCamera *pThis,
                                    const TPos *p1_c, const TPos *p2_c,
                                    const TPlaneIdx *clipPlanes);
BOOL8 Camera_QuickIsVisibleLine2d_w(const TCamera *pThis, const TPos *p1_w, const TPos *p2_w,
                                    const TPlaneIdx *clipPlanes);
BOOL8 Camera_QuickIsVisiblePoint2dWithMargin_c(const TCamera *pThis, const TPos *p_c,
                                               const TPlaneIdx *clipPlanes,
                                               INT32 margin_m);
BOOL8 Camera_QuickIsVisiblePoint2dWithMargin_w(TCamera *pThis, const TPos *p_w,
                                               const TPlaneIdx *clipPlanes,
                                               INT32 margin_m);
BOOL8 Camera_QuickIsVisiblePoint2d_c(const TCamera *pThis, const TPos *p_c,
                                     const TPlaneIdx *clipPlanes);
BOOL8 Camera_QuickIsVisiblePoint2d_w(const TCamera *pThis, const TPos *p_w,
                                     const TPlaneIdx *clipPlanes);
BOOL8 Camera_QuickIsVisibleRect_w(const TCamera *pThis,
                                  const TRect *bounds_w,
                                  const TPlaneIdx *clipPlanes);
void Camera_RefreshCamera(TCamera *pThis);
void Camera_Screen2dToWorld2d(const TCamera *pThis,
                              TPos *pos_w, const TPos *pos_s);
void Camera_ScreenToMap(const TCamera *pThis,
                        INT32 *outx, INT32 *outy,
                        INT32 inx, INT32 iny);
void Camera_World2dToScreen2d(const TCamera *pThis,
                              TPos *pos_s, const TPos *pos_w);

// END PRIVATE

#ifdef __cplusplus
}
#endif

#endif /* DI3DENGIP_H */
