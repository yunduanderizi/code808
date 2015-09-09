#ifndef MAPCORE_ROUTING_H
#define MAPCORE_ROUTING_H

//## This API is still in a state of flux.
//## Comments on the API are interleaved with API itself.  Such
//## comments are prepended with `//##'

/// @addtogroup Routing
/// This module computes paths between the destinations of an itinerary.
/// 
/// The application must build up an itinerary, and then pass it to the
/// journey calculator with the desired routing options.  The journey
/// calculator when iterated will determine a journey the matches the
/// specified itinerary.
///
/// The calculated journey is made up of a series of legs, one for each
/// destination in the itinerary.  These legs contain lists of road segments
/// showing how to travel through the road network from the leg origin to
/// the leg destination.
///
/// Journeys can displayed by the core display subsystem, and the user given
/// visual and audio guidence to assist their travel along a journey.
/// @{

#include <routing/rtapibase.h>
#include <routing/rtbanareas.h>
#include <routing/rtbans.h>
#include <routing/rtbansegments.h>
#include <routing/rtitinerary.h>
#include <routing/rtjourney.h>
#include <routing/rtjourneycalc.h>
#include <routing/rtloadsave.h>
#include <routing/rtoptions.h>
#include <routing/rtpath.h>
#include <routing/rtpathcalc.h>
#include <routing/rtroadsegment.h>
#include <routing/rtsegmentlist.h>

/// @}

#endif /* MAPCORE_ROUTING_H */

/* End of mapcore_routing.h */
