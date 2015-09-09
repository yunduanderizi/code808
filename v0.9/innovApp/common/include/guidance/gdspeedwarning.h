#ifndef GDSPEEDWARNING_H
#define GDSPEEDWARNING_H

#include <mapdata/enroad.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Guidance
/// @{

/// @addtogroup SpeedWarning
/// @ingroup Guidance
/// @{

/// The default speed in excess of the speed limit that must be exceeded
/// to trigger additional warnings (km/h)
#define DEFAULT_WARNING_SPEED_THRESHOLD 10

/// The default time delay in seconds once the speed limit has been exceeded
/// before a warning is triggered
#define DEFAULT_WARNING_TIME_THRESHOLD 3


/// This is the form of the callback function that is called when a speed
/// warning is triggered.
/// @param [in] speed The current speed
typedef void TSpeedWarningFunc(INT32 speed);


/// Speed / safety warning. The speed warning provides the ability to alert
/// the user when the speed limit has been exceeded. A speed warning is
/// associated with a particular #TGuideContext and may be accessed using
/// #guideContext_getSpeedWarning.
typedef struct {
  /// @internal
  /// Callback function for when the speed warning is triggered
  TSpeedWarningFunc* warningFunc;
  /// @internal
  /// The UTC time in ms when the speed limit was initially exceeded
  INT32 timeOverLimit;
  /// @internal
  /// The UTC time in ms when the last speed warning was triggered
  INT32 lastWarningTime;
  /// @internal
  /// Threshold speed in km/h over the speed limit at which additional
  /// warnings are triggered
  UINT16 speedThreshold;
  /// @internal
  /// Duration in ms the speed limit must exceeded for before the first speed
  /// warning is triggered
  UINT16 timeThreshold;
  /// @internal
  /// Delay in ms between repeated warnings while the speed limit is exceeded
  UINT16 warningRepeatTime;
  /// @internal
  /// The speed in km/h at the time the last speed warning was triggered
  UINT16 lastWarningSpeed;
  /// @internal
  /// Speed limit settings for each road class
  UINT16 speedLimits[ROAD_TYPES];
} TSpeedWarning;

/*----------------------------------------------------------------*/

extern MAPCORE_API
void speedWarning_installFunction(TSpeedWarning *pThis,
                                  TSpeedWarningFunc* func);

extern MAPCORE_API
void speedWarning_setWarningSpeed(TSpeedWarning *pThis,
                                  TRoadType roadType,
                                  UINT16 speed);

extern MAPCORE_API
UINT16 speedWarning_getWarningSpeed(const TSpeedWarning *pThis,
                                    TRoadType roadType);

extern MAPCORE_API
void speedWarning_setMotorwayWarningSpeed(TSpeedWarning *pThis,
                                          UINT16 speed);

extern MAPCORE_API
void speedWarning_setMajorRoadWarningSpeed(TSpeedWarning *pThis,
                                           UINT16 speed);

extern MAPCORE_API
void speedWarning_setMinorRoadWarningSpeed(TSpeedWarning *pThis,
                                           UINT16 speed);

extern MAPCORE_API
void speedWarning_setAllWarningSpeeds(TSpeedWarning *pThis,
                                      UINT16 speed);

extern MAPCORE_API
void speedWarning_setWarningSpeedThreshold(TSpeedWarning *pThis,
                                           UINT16 speed);

extern MAPCORE_API
UINT16 speedWarning_getWarningSpeedThreshold(const TSpeedWarning *pThis);

extern MAPCORE_API
void speedWarning_setWarningTimeThreshold(TSpeedWarning *pThis,
                                                 UINT16 time );

extern MAPCORE_API
UINT16 speedWarning_getWarningTimeThreshold(const TSpeedWarning *pThis);

extern MAPCORE_API
void speedWarning_setWarningRepeatTime(TSpeedWarning *pThis,
                                              UINT16 time);

extern MAPCORE_API
UINT16 speedWarning_getWarningRepeatTime(const TSpeedWarning *pThis);

/// @}

/// @}

#ifdef __cplusplus
}
#endif

#endif /* GDSPEEDWARNING_H */
