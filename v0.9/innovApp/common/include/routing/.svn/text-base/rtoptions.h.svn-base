//
// Declaration of the path calculation options type and its associated
// functions
//
// $Id: rtoptions.h 10771 2008-08-02 12:13:15Z Woody.lee $
//

#ifndef RTOPTIONS_H
#define RTOPTIONS_H

#include <platform_types.h>
#include <mapdata/enroad.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @addtogroup Routing
/// @{
/// @addtogroup RoutingOptions
/// @ingroup Routing
/// @{


enum TVehicleType;              /* forward declaration */

/// @typedef  struct TRoutingOptions
/// These preferences allow the cost function and constraints applied by
/// the routing algorithm to be manipulated.
///
/// A set of routing options must be passed to the journey calculator when
/// it is initialised. To fill in the structure, please use
/// routingOptions_setToPreset() or routingOptions_setToDefaults().
/// 
/// To set additional preferences please use the routingOptions_set...Preference()
/// functions. 
///
/// @note The terms 'freeway' and 'motorway' used in this context follow the
/// TeleAtlas definitions:
///   - (Road form) 'Freeway' represents the fact that a road for at least 2 km
///     crosses other roads at a different level and is connected with the rest
///     of the road network by means of major slip roads except at its start and
///     end.
///   - (Form of way) 'Motorway' represents the fact that a road is officially
///     assigned as motorway. E.g. 'A' autobahnen in Germany and 'M' motorways
///     in the UK. (The road class 'motorway' is in general pretty useless, as
///     in the map data it's even assigned to smaller roads. The road class is
///     more a representation of the importance of a road - according to TeleAtlas)
///
/// That means, that a 'freeway' is a big and important road, with special rules
/// that for example forbid the usage by bicycles or pedestrians.
typedef struct TRoutingOptions {

  /// Enumerated type of vehicle / mode of transport. Passenger car or
  /// pedestrian.
  enum TVehicleType vehicleType;

  /// This value allows routing to be continuously adjusted between
  /// 'fastest' time and 'shortest' distance.
  /// - 0 (default) = calculate routes that tend to be faster (they take
  ///   full account of the speed of different roads).
  /// - 255 = calculate routes that tend to be shorter (the speeds of
  ///   different roads are assumed to be identical)
  /// It doesn't apply to turn costs. 
  ///
  /// This option has precedence over the four following preference
  /// options.  If the shortestDistancePreference is set to absolute
  /// shortest, the freeway, unsealed, urban and ferry road speed
  /// preferences will have no effect (ferry entry costs, and the avoid
  /// tolls/unsealed/ferries flags will still apply).
  UINT8 shortestDistancePreference;

  /// This option allows continuous adjustment of freeway road speeds from
  /// the average speed given in the data (or from 120% of the default
  /// sealed speeds if real averages are not available)
  /// - 0 = freeways will act as though they run at 80% of their stated
  ///       average speed
  /// - 128 (default) = freeways get a bonus and act as though they run at
  ///       120% of their average speed
  /// - 255 = freeways are much preferred highly over normal roads and act
  ///       as though they run at 160% of their average speed
  UINT8 freewayUsagePreference;

  /// This option allows continuous adjustment of expressway road speeds from
  /// the average speed given in the data (or from 120% of the default
  /// sealed speeds if real averages are not available)
  /// The expressway is defined currently as ROAD_TYPE_REGIONAL_HIGHWAY
  /// - 0 = expressways will act as though they run at 80% of their stated
  ///       average speed
  /// - 128 (default) = expressways get a bonus and act as though they run at
  ///       120% of their average speed
  /// - 255 = expressways are much preferred highly over normal roads and act
  ///       as though they run at 160% of their average speed
  UINT8 expresswayUsagePreference;

  /// This is special routing option for Taiwan only.
  /// The highway No.1 is the one of freeways in Taiwan.
  /// This option allows continuous adjustment of highway No.1 speeds from
  /// the average speed given in the data (or from 120% of the default
  /// sealed speeds if real averages are not available)
  /// - 0 = highway No.1 will act as though they run at 80% of their stated
  ///       average speed
  /// - 128 (default) = highway No.1 get a bonus and act as though they run at
  ///       120% of their average speed
  /// - 255 = highway No.1 are much preferred highly over normal roads and act
  ///       as though they run at 160% of their average speed
  UINT8 highwayNo1UsagePreference;

  /// This is special routing option for Taiwan only.
  /// The highway No.3 is the one of freeways in Taiwan.
  /// This option allows continuous adjustment of highway No.3 speeds from
  /// the average speed given in the data (or from 120% of the default
  /// sealed speeds if real averages are not available)
  /// - 0 = highway No.3 will act as though they run at 80% of their stated
  ///       average speed
  /// - 128 (default) = highway No.3 get a bonus and act as though they run at
  ///       120% of their average speed
  /// - 255 = highway No.3 are much preferred highly over normal roads and act
  ///       as though they run at 160% of their average speed
  UINT8 highwayNo3UsagePreference;

  /// This option allows continuous adjustment of unsealed road speeds from
  /// very slow up to their average speed in the data (or 80% of the
  /// default sealed speeds if real averages are not available)
  /// - 0 (default) = unsealed roads have a 2x cost penalty
  /// - 255 = unsealed roads have no penalty and use the correct average
  ///       speed
  UINT8 unsealedUsagePreference;

  /// Specify a speed penalty to apply to urban roads
  /// - 0 (default) = urban roads have no speed penalty
  /// - 255 = urban roads are two times slower than non urban roads of
  ///       identical class and will therefore tend to be avoided
  UINT8 urbanAvoidancePreference;

  /// Configure the cost of ferries
  /// - 0 = ferries are very slow and have a very large entry penalty
  /// - 255 (default) = ferries travel at their stated average speed and
  ///       have no entry cost
  UINT8 ferryUsagePreference;

  /// Configure calculation of turning costs
  /// - 0   = calculate routes that can have lots of turns
  /// - 128 = use the standard weights that have been before this parameter
  ///         has been introduced (default)
  /// - 255 = try to avoid expensive turns as much as possible
  ///         (turn costs are 4 times higher than the default value)
  UINT8 turnCostPreference;

  /// Specify absolute toll road avoidance
  /// - FALSE (default) = toll roads are used with no penalty.
  /// - TRUE = toll roads are completely avoided.
  BOOL8 avoidTollsFlag;

  /// This allows the absolute avoidance of unsealed roads to be specified.
  /// - FALSE = unsealed roads roads are allowed, but are (by default) more
  ///   expensive than normal roads.
  /// - TRUE (default) = unsealed roads are completely avoided.
  BOOL8 avoidUnsealedFlag;

  /// specify absolute ferry avoidance
  /// - FALSE = ferry links can be used (but may be expensive).
  /// - TRUE (default) = strictly no ferry links are used.
  BOOL8 avoidFerriesFlag;

  /// Specify whether routing should try to algorithmically avoid dual
  /// carriageway u-turn.  This preference can also be set by a global bit
  /// in the map data for a DCA.  The most restrictive option is used.  If
  /// enabled detected dual-carriageway u-turns are absolutely avoided but
  /// not all of them will be detected.
  /// - FALSE (default) = no penalty.
  /// - TRUE = completely avoided when detected.
  BOOL8 avoidDualCarriagewayUTurnsFlag;

  /// Specify absolute freeway avoidance. This option should be set, if the
  /// user doesn't want to drive on a freeway/motorway.
  /// - FALSE (default) freeways can be used.
  /// - TRUE do not use freeways unless leaving or entering a freeway.
  BOOL8 avoidFreewaysFlag;

  /// Specify overpass avoidance. This option should be set, if the
  /// user doesn't want to drive on an overpass.
  /// - FALSE (default) overpass can be used.
  /// - TRUE do not use overpass unless leaving or entering an overpass.
  BOOL8 avoidOverpassFlag;

  /// Specify expressway avoidance. This option should be set, if the
  /// user doesn't want to drive on an expressway.
  /// - FALSE (default) expressway can be used.
  /// - TRUE do not use expressway unless leaving or entering an expressway.
  BOOL8 avoidExpresswayFlag;
} TRoutingOptions;


/// These are the different routing presets, that are currently available. It
/// would be nice to allow the user to create new ones, but that should be
/// handled in SmartST.
typedef enum {
  /// Routing tries to find the route with the shortest driving time.
  RT_OPTION_CAR_FASTEST = 0,
  /// Routing tries to find the very shortest route.
  RT_OPTION_CAR_SHORTEST,
  /// Routing tries to find a route that's easier to drive on.
  RT_OPTION_CAR_EASIEST,
  /// Routing tries to find a route that minimises fuel consumption.
  RT_OPTION_CAR_ECONOMICAL,
  /// Routing tries to fine a route with expressway first.
  RT_OPTION_CAR_EXPRESSWAY,
  /// Routing tries to find a route with Highway No.1 first.
  /// This is a special routing option for Taiwan only.
  RT_OPTION_CAR_HIGHWAYNO1,
  /// Routing tries to find a route with Highway No.3 first.
  /// This is a special routing option for Taiwan only.
  RT_OPTION_CAR_HIGHWAYNO3,
  /// Routing calculates routes that are useful and usable for pedestrians.
  RT_OPTION_PEDESTRIAN,

  /// The number of available presets (must be last)
  RT_OPTION_PRESETS
} TRoutingOptionPreset;


/// These are the different possibilities to modify routing behaviour.
typedef enum {
  /// Allow the specific road type, but don't change the costs.
  RT_OPTION_PREF_ALLOW = 0,
  /// Try to avoid a specific road type by increasing the costs.
  RT_OPTION_PREF_TRY_TO_AVOID,
  /// Forbid a specific road type.
  RT_OPTION_PREF_AVOID,

  /// The number of available preference settings (must be last)
  RT_OPTION_PREFERENCES
} TRoutingOptionPreference;


/*----------------------------------------------------------------*/
/* ROUTING OPTIONS APIs */
/*----------------------------------------------------------------*/

/// Set a routing options object to the defaults suggested by the core for
/// best routing.
///
/// @param[in/out]  pThis
///   Pointer to the structure into which the routing options will be written.
/// @param[in]      vehicleType
///   The vehicle type for which the routing options should be set.
extern MAPCORE_API
void routingOptions_setToDefaults(
  TRoutingOptions *pThis,
  TVehicleType    vehicleType );

/// Set routing options to a specific preset.
///
/// @param[in/out]  pThis
///   Pointer to the structure into which the routing options will be written.
/// @param[in]      preset
///   The routing option preset that should be used.
extern MAPCORE_API
void routingOptions_setToPreset(
  TRoutingOptions      *pThis, 
  TRoutingOptionPreset preset );


/// Returns TRUE if you can change the toll road preference for the current
/// preset.
extern MAPCORE_API
BOOL8 routingOptions_tollRoadPreferenceAvailable( TRoutingOptions *pThis );

/// Sets the routing behaviour for toll roads.
///
/// @param[in/out]  pThis
///   Pointer to the structure containing the options to be changed.
/// @param[in]      preference
///   The routing behaviour you want for toll roads.
extern MAPCORE_API
void routingOptions_setTollRoadPreference(
  TRoutingOptions          *pThis,
  TRoutingOptionPreference preference );


/// Returns TRUE if you can change the unsealed road preference for the current
/// preset.
extern MAPCORE_API
BOOL8 routingOptions_unsealedRoadPreferenceAvailable( TRoutingOptions *pThis );

/// Sets the routing behaviour for unsealed roads.
///
/// @param[in/out]  pThis
///   Pointer to the structure containing the options to be changed.
/// @param[in]      preference
///   The routing behaviour you want for unsealed roads.
extern MAPCORE_API
void routingOptions_setUnsealedRoadPreference(
  TRoutingOptions          *pThis,
  TRoutingOptionPreference preference );


/// Returns TRUE if you can change the ferry preference for the current
/// preset.
extern MAPCORE_API
BOOL8 routingOptions_ferryPreferenceAvailable( TRoutingOptions *pThis );

/// Sets the routing behaviour for ferries.
///
/// @param[in/out]  pThis
///   Pointer to the structure containing the options to be changed.
/// @param[in]      preference
///   The routing behaviour you want for ferries.
extern MAPCORE_API
void routingOptions_setFerryPreference(
  TRoutingOptions          *pThis,
  TRoutingOptionPreference preference );


/// Returns TRUE if you can change the ferry preference for the current
/// preset.
extern MAPCORE_API
BOOL8 routingOptions_motorwayPreferenceAvailable( TRoutingOptions *pThis );

/// Sets the routing behaviour for freeways/motorways.
///
/// @param[in/out]  pThis
///   Pointer to the structure containing the options to be changed.
/// @param[in]      preference
///   The routing behaviour you want for freeways/motorways.
extern MAPCORE_API
void routingOptions_setMotorwayPreference(
  TRoutingOptions          *pThis,
  TRoutingOptionPreference preference );

/// Sets the routing behaviour for dual Carriageway UTurns road.
///
/// @param[in/out]  pThis
///   Pointer to the structure containing the options to be changed.
/// @param[in]      preference
///   The routing behaviour you want for Carriageway UTurns road..
extern MAPCORE_API
void routingOptions_setDCUturnsPreference(
	TRoutingOptions          *pThis,
	TRoutingOptionPreference preference );

/// Sets the routing behaviour for overpass road.
///
/// @param[in/out]  pThis
///   Pointer to the structure containing the options to be changed.
/// @param[in]      preference
///   The routing behaviour you want for overpass road.
extern MAPCORE_API
void routingOptions_setOverpassRoadPreference(
	TRoutingOptions          *pThis,
	TRoutingOptionPreference preference );

/// Sets the routing behaviour for Expressway Road
///
/// @param[in/out]  pThis
///   Pointer to the structure containing the options to be changed.
/// @param[in]      preference
///   The routing behaviour you want for Expressway Road
extern MAPCORE_API
void routingOptions_setExpresswayRoadPreference(
	TRoutingOptions          *pThis,
	TRoutingOptionPreference preference );


/// @}
/// @}
#ifdef __cplusplus
}
#endif

#endif // RTOPTIONS_H
