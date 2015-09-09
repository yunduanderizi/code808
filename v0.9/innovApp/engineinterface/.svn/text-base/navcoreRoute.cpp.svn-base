#include <innovEngine_logging.h>
#include <innovEngine_display.h>
#include "engineinterface/engineinterface.h"
#include "navcoreRoute.h"
#include "interface_guide.h"
TArchiveOverlaySet g_overlaySet;

#define LOGSCOPE LOG_APPLICATION

extern void NavtestDisplay_JunctionView(UINT32 event, TJunctionViewData  junctionViewData, void *appContext);

void NavtestDisplay_JunctionView(UINT32 event, TJunctionViewData  junctionViewData, void *appContext)
{
    switch(event)
    {
    case JUNCTION_VIEW_DISPLAY_EVENT:
        archive_resolveJunctionView(&g_overlaySet, junctionViewData, /*g_isNight*/false);
        setishavejuctionchange(1);
        //g_mapjunctionview->refresh();
        break;
    case JUNCTION_VIEW_RESET_EVENT:
        setishavejuctionchange(0);
        //g_mapjunctionview->clear();
        break;
    default:
        break;
    }
}

navcoreRoute::navcoreRoute(void)
: m_tempJourney(NULL)
, m_definingNewJourney(false)
, m_isSimulating(false)
, m_simulatorSpeed(28)
, m_simulatorRate(0)
, m_isSimulationFinished(false)
, m_timeOffRoute(0)
, m_ismanualSimulating(false)
, m_ismanualSimulateFinished(false)
{
  m_currentJourney.clear();
  m_newJourney.clear();
  m_routeNames.clear();
}

navcoreRoute::~navcoreRoute(void)
{
  m_definingNewJourney = false;
}

// m_definingNewJourney == false: startNewRoute()
//                         -> 1. Construct m_tempJourney
//                            2. m_definingJourney = true && m_newJourney = newRouteName
//                            3. Define a new start point for this journey
//                            4. Return true
// m_definingNewJourney == true : startNewRoute() && m_newJourney == newRoutename
//                            1. Define a new start point for this journey
//                            2. Return true
// m_definingNewJourney == true : startNewRoute() && m_newJourney != newRoutename
//                         -> put up message box: Need to completely define new route and return false
bool navcoreRoute::startNewRoute(const TPos& mapPos,
                                        TMapLocation * pMapLoc)
{
    delete m_tempJourney;
    m_tempJourney = new navcoreJourney();
    m_definingNewJourney = false;
    pMapLoc ? m_tempJourney->setAsStart(pMapLoc) : m_tempJourney->setAsStart(mapPos);

}

// m_definingNewJourney == false: addIntermediateToNewRoute()
//                         -> return false
// m_definingNewJourney == true : addIntermediateToNewRoute()
//                         -> add to itinerary and return true.
bool navcoreRoute::addIntermediateToNewRoute(const TPos& mapPos,
                                                    const TRoutingOptions& options,
                                                    TMapLocation * pMapLoc)
{
  if (m_definingNewJourney) {
    return pMapLoc ? m_tempJourney->setAsDestination(pMapLoc, options) : m_tempJourney->setAsDestination(mapPos, options);
  } else { // route currently not being defined...
    return false;
  }
}

bool navcoreRoute::GetCurrentJourney(navcoreJourney *pJourney)
{
  if(m_tempJourney != NULL)
  {
    pJourney = m_tempJourney;
    return true;
  }else
  {
    return false;
  }
}

TPos * navcoreRoute::GetStartPoint()
{
  // todo: use maplocation_getNetworkPos instead
    if(NULL !=m_tempJourney )
        return &m_tempJourney->GetStartPt()->user_pos;
    else
        return NULL;
}

bool navcoreRoute::GetDestPoint(TItineraryDestNumber destNum, TPos& pos)
{
  // todo: use maplocation_getNetworkPos instead
  return m_tempJourney->getDestPos(destNum, pos);
}


map<wstring, navcoreJourney, less<wstring> > * navcoreRoute::GetJourneys()
{
  return &m_journeyMap;
}

// m_definingNewJourney == false: endNewRoute()
//                         -> return false
// m_definingNewJourney == true : endNewRoute()
//                         -> 1. Add to itinerary
//                            2. Calculate route
//                            3. Calculate guidance list
//                            4. Add to m_journeyMap if all OK
//                            5. m_definingNewJourney = false
//                            6. Delete m_tempJourney
//                            7. Return true.
bool navcoreRoute::endNewRoute(const TPos& mapPos,
                                      const TRoutingOptions& options,
                                      TMapLocation * pMapLoc)
{
    //  if (m_definingNewJourney) {
    if (pMapLoc ? m_tempJourney->setAsDestination(pMapLoc, options) : m_tempJourney->setAsDestination(mapPos, options)) {

        if (m_tempJourney->calculateJourney()) {
            // Calculate guidance for this route
            if (m_tempJourney->calculateGuidance()) {
                // Add to the NamedRoute map (m_journeyMap)
                //
                // The line below was previously a m_journeyMap.insert() call.
                // This worked fine on my machine but caused runtime problems
                // that broke this multi-routing feature in navtest.  Using the
                // STL map [] operator seems to fix this issue on both Hayden and
                // Sajeewa's case and causes no regression on my machine.
                //
                //          m_journeyMap[m_newJourney] = *m_tempJourney;

                guideContext_registerJunctionViewCallback((*m_tempJourney).getGuideContext(), NavtestDisplay_JunctionView, 0);

                // Take the current setting of the measurement system
                (*m_tempJourney).setGuidanceMeasurementSystem(m_unitsOfMeasurement);

//                delete m_tempJourney;
//                m_tempJourney = NULL;
                m_definingNewJourney = true;
            }
            else{return false;}
        }
        else
        {
            LOG_ERROR(0, ("Could not calculate route"));
            m_definingNewJourney = false;
            return false;}
    }
    else {return false;}
}

// Retrieve the routing preferences of the currently selected route.  This has
// to be invoked with a guard by the caller to check whether there is a route
// currently selected.
bool navcoreRoute::getCurrentRouteNameRoutingPreferences(TRoutingOptions& options,
                                                                TItineraryDestNumber destNum)
{
  if (m_currentJourney.empty() ||
      !m_journeyMap[m_currentJourney].getRoutingOptions(destNum, options)) {
    return false;
  }

  return true;
}

// Set the routing preferences of the currently selected route.
bool navcoreRoute::setCurrentRouteNameRoutingPreferences(const TRoutingOptions& options,
                                                                TItineraryDestNumber destNum)
{
  if (m_currentJourney.empty()) {
    LOG_ERROR(0, ("Cannot set route preferences, no route currently selected"));
    return false;
  }

  if (!m_journeyMap[m_currentJourney].setRoutingOptions(destNum, options)) {
    return false;
  }

  return true;
}

// Are we currently defining a new journey?
bool navcoreRoute::isDefiningNewJourney(void)
{
  return m_definingNewJourney;
}

// Render any calculated routes
void navcoreRoute::renderAllJourneys(TViewport * viewport)
{
    if(m_tempJourney != NULL)
    {
        Journey_Render(m_tempJourney->getJourney(), viewport);
        GuideContext_Render(m_tempJourney->getGuideContext(), viewport);
    }
}

// Render any calculated journey flags
void navcoreRoute::renderAllJourneyFlags(TViewport * viewport)
{
    if(m_tempJourney != NULL)
    {
        Journey_RenderFlags(m_tempJourney->getJourney(), viewport);
    }
}

// Render any guidance for calculated routes
void navcoreRoute::renderAllGuidanceLists(TViewport * viewport)
{
  NamedJourney::iterator i = m_journeyMap.begin();

  while (i != m_journeyMap.end()) {
    if(i->second.IfShowOnMap()) GuideContext_Render((*i).second.getGuideContext(), viewport);
    i++;
  }
}

// This is the name of the currently selected (active) journey
bool navcoreRoute::setCurrentRouteName(const wstring& selectedRouteName)
{
  NamedJourney::const_iterator i = m_journeyMap.find(selectedRouteName);

  if (i != m_journeyMap.end()) { // Paranoia...  This shouldn't really have to checked...
    m_currentJourney = selectedRouteName;

    // Apply the current measurement system it could have changed...
    m_journeyMap[m_currentJourney].setGuidanceMeasurementSystem(m_unitsOfMeasurement);

    return true;
  } else {
    LOG_ERROR(0, ("Attempted to select a journey not present(!)"));
//    AfxMessageBox(L"Attempted to select a journey not present(!)",
//                  MB_OK | MB_ICONEXCLAMATION);

    return false;
  }
}

// No current journey selected
void navcoreRoute::unsetCurrentRouteName(void)
{
  m_currentJourney.clear();
}

// The currently selected "active" route
const wstring& navcoreRoute::getCurrentRouteName(void)
{
  return m_currentJourney;
}

// The number of defined routes
const unsigned int navcoreRoute::getNumberOfRoutes(void)
{
  return (unsigned int)m_journeyMap.size();
}

// Save the current selected route
bool navcoreRoute::saveRoute(const wstring& pathName)
{
  if (!m_currentJourney.empty()) {
    //USES_CONVERSION;
    //return m_journeyMap[m_currentJourney].saveJourney(W2CA(pathName.c_str()));
  } else {
    LOG_ERROR(0, ("No route currently selected"));

    return false;
  }
}

// Load a route from a file
bool navcoreRoute::loadRoute(const wstring& pathName, const wstring& routeName)
{
  NamedJourney::const_iterator i = m_journeyMap.find(routeName);

  if (i != m_journeyMap.end()) {
    LOG_ERROR(0, ("Route by this name already exists, '%s'", routeName.c_str()));

    return false;
  } else {
    // I can only get this going on the heap.  Does not work on the stack for
    // object t.  Why is this?
    navcoreJourney *t = new navcoreJourney;
    if (t != NULL) {
      //USES_CONVERSION;
      bool rc;// = t->loadJourney(W2CA(pathName.c_str()));

      if (rc == true) {
        // Map it in...
        m_journeyMap[routeName] = *t;
      } else {
        LOG_ERROR(0, ("Route \"%s\" could not be loaded", routeName.c_str()));
      }
      delete t;

      return rc;
    } else {
      LOG_ERROR(0, ("Route \"%s\" could not be loaded: "
        "could not allocate navcoreJourney object", routeName.c_str()));

      return false;
    }
  }
}

// Using the currently selected route, clone the journey giving it the name, newRouteName
bool navcoreRoute::cloneRoute(const wstring& newRouteName)
{
  if (!m_currentJourney.empty()) {
    m_journeyMap[newRouteName] = m_journeyMap[m_currentJourney];
    return true;
  } else {
    LOG_ERROR(0, ("Could not clone journey, no current journey selected."))
    return false;
  }
}

// Remove the currently selected route
bool navcoreRoute::removeRoute(void)
{
    delete m_tempJourney;
    m_tempJourney = NULL;
    m_definingNewJourney = false;
    // This is done to ensure that maplock doesn't access a stale journey.
    maplock_setJourney(NULL);

}

// Remove all routes
bool navcoreRoute::removeAllRoutes(void)
{
  m_journeyMap.clear();

  // This is done to ensure that maplock doesn't access a stale journey.
  maplock_setJourney(NULL);

  return true;
}

// Save the currently selected route as an XML testcase
bool navcoreRoute::saveXMLTestCase(const wstring& pathName)
{
  //USES_CONVERSION;
  //m_journeyMap[m_currentJourney].writeXMLRoutingTestCase(W2CA(pathName.c_str()));

  return true;
}

unsigned int navcoreRoute::getTravelTime(void)
{
  if (!m_currentJourney.empty()) {
    return m_journeyMap[m_currentJourney].getTravelTime();
  } else {
    LOG_ERROR(0, ("No current journey: Cannot retrieve travel time"));

    return 0;
  }
}

unsigned int navcoreRoute::getNumberOfTurns(void)
{
  if (m_tempJourney!=NULL) {
    return m_tempJourney->getNumberOfTurns();
  } else {
    LOG_ERROR(0, ("No current journey: Cannot retrieve number of turns"));

    return 0;
  }
}

unsigned int navcoreRoute::getNumberOfInstructions(void)
{
  if (m_tempJourney != NULL) {
    return m_tempJourney->getNumberOfInstructions();
  } else {
    LOG_ERROR(0, ("No current journey: Cannot retrieve instruction count"));

    return 0;
  }
}

unsigned int navcoreRoute::getRouteLength(void)
{
  if (!m_currentJourney.empty()) {
    return m_journeyMap[m_currentJourney].getRouteLength();
  } else {
    LOG_ERROR(0, ("No current journey: Cannot retrieve route length"));

    return 0;
  }
}

bool navcoreRoute::getGuidanceLocation(unsigned int index, TPos& mapPos)
{
  if (!m_currentJourney.empty()) {
    return m_journeyMap[m_currentJourney].getGuidanceLocation(index, mapPos);
  } else {
    LOG_ERROR(0, ("No current journey: Cannot retrieve guidance location"));

    return false;
  }
}

bool navcoreRoute::getGuidanceSourceHeading(unsigned int index, UINT8& heading)
{
  if (!m_currentJourney.empty()) {
    return m_journeyMap[m_currentJourney].getGuidanceSourceHeading(index, heading);
  } else {
    LOG_ERROR(0, ("No current journey: Cannot retrieve guidance location"));

    return false;
  }
}

bool navcoreRoute::initialiseIntersectionPictorial(unsigned int index)
{
  if (m_tempJourney!=NULL) {
    return m_tempJourney->initialiseIntersectionPictorial(index);
  } else {
    LOG_ERROR(0, ("No current journey: Cannot initialise intersection pictorial"));

    return false;
  }
}

  // Return intersection attributes.  This assumes that:
  // a) A valid journey and guidance list exist AND
  // b) That initialiseIntersectionPictorial() for given index in the
  //    guidance list has been initialised.
  // If neither of these conditions are met then return false.
bool navcoreRoute::retrieveIntersectionPictorialAttributes(navcoreJourney::TIntersectionDescr& intDescr)
{
  if (m_tempJourney!=NULL) {
    return m_tempJourney->retrieveIntersectionPictorialAttributes(intDescr);
  } else {
    LOG_ERROR(0, ("No current journey: Cannot retrieve intersection pictorial attributes"));

    return false;
  }
}

bool navcoreRoute::getGuidanceInstruction(const unsigned int guideIndex,
                                                 wstring& distanceFromStart,
                                                 wstring& instruction,
                                                 wstring& grade,
                                                 wstring& style)
{
  if (m_tempJourney != NULL) {
    return m_tempJourney->getGuidanceInstruction(guideIndex, distanceFromStart, instruction, grade, style);
  } else {
    LOG_ERROR(0, ("No current journey: Cannot retrieve guidance instruction"));

    return false;
  }
}

bool navcoreRoute::getGuidanceInfo(const unsigned int guideIndex,
                                                 wstring& distanceFromStart,
                                                 wstring& roadname,
                                                 wstring& grade,
                                                 wstring& style,
                                                 TSegmentID &nsegmentid)
{
  if (m_tempJourney != NULL) {
    return m_tempJourney->getGuidanceInfo(guideIndex, distanceFromStart, roadname, grade, style, nsegmentid);
  } else {
    LOG_ERROR(0, ("No current journey: Cannot retrieve guidance instruction"));

    return false;
  }
}

bool navcoreRoute::getGuidanceInstruction(const unsigned int guideIndex,
                                                 UTF16* instructionText)
{
  if (m_tempJourney != NULL) {
    return m_tempJourney->getGuidanceInstruction(guideIndex, instructionText);
  } else {
    LOG_ERROR(0, ("No current journey: Cannot retrieve guidance instruction"));

    return false;
  }
}

TGuideInstruction * navcoreRoute::getGuidanceInstruction(const unsigned int guideIndex)
{
  if (m_tempJourney != NULL) {
    return m_tempJourney->getGuidanceInstruction(guideIndex);
  }

  LOG_ERROR(0, ("No current journey: Cannot retrieve guidance instruction"));
  return NULL;
}

TGuideList * navcoreRoute::getGuideList(void)
{
  if (m_tempJourney != NULL) {
    return m_tempJourney->getGuidanceList();
  }

  LOG_ERROR(0, ("No current journey: Cannot retrieve guidance list"));
  return NULL;
}

TGuideContext * navcoreRoute::getGuideContext(void)
{
  if (m_tempJourney != NULL) {
    return m_tempJourney->getGuideContext();
  }

  LOG_ERROR(0, ("No current journey: Cannot retrieve guidance context"));
  return NULL;
}


// Return the n-th key (specified by index) of the container
const vector<wstring>& navcoreRoute::getRouteNames(void)
{
  NamedJourney::const_iterator i = m_journeyMap.begin();

  m_routeNames.clear();
  while (i != m_journeyMap.end()) {
    m_routeNames.push_back((*i).first);
    i++;
  }

  return m_routeNames;
}

unsigned int navcoreRoute::getNumberOfJourneyLegs(void)
{
  if (!m_currentJourney.empty()) {
    return m_journeyMap[m_currentJourney].getNumberOfJourneyLegs();
  } else {
    LOG_ERROR(0, ("No current journey: Cannot retrieve journey leg count"));

    return 0;
  }
}

// Starting point of itinerary (not really a part of the itinerary as such)
bool navcoreRoute::getItineraryStartInfo(wstring& location, wstring& streetname)
{
  if (!m_currentJourney.empty()) {
    return m_journeyMap[m_currentJourney].getItineraryStartInfo(location, streetname);
  } else {
    location.clear();
    streetname.clear();

    return false;
  }
}

bool navcoreRoute::getItineraryStartLocation(TPos& mapPos)
{
  if (!m_currentJourney.empty()) {
    return m_journeyMap[m_currentJourney].getItineraryStartLocation(mapPos);
  } else {
    mapPos.x = GPS_BADVALUE;
    mapPos.y = GPS_BADVALUE;

    return false;
  }
}

// A intermediate destination (and final destination) on the journey
bool navcoreRoute::getItineraryDestInfo(TItineraryDestNumber destNum,
                                               wstring& location,
                                               wstring& streetname)
{
  if (!m_currentJourney.empty()) {
    return m_journeyMap[m_currentJourney].getItineraryDestinationInfo(destNum, location, streetname);
  } else {
    location.clear();
    streetname.clear();

    return false;
  }
}

bool navcoreRoute::getItineraryDestLocation(TItineraryDestNumber destNum,
                                                   TPos& mapPos)
{
  if (!m_currentJourney.empty()) {
    return m_journeyMap[m_currentJourney].getItineraryDestinationLocation(destNum, mapPos);
  } else {
    mapPos.x = GPS_BADVALUE;
    mapPos.y = GPS_BADVALUE;
    return false;
  }
}

// Set the measurement system (variations on metric and imperial)
void navcoreRoute::setGuidanceMeasureSystem(const TDistanceCombination unitsOfMeasurement)
{
  m_unitsOfMeasurement = unitsOfMeasurement;

  if (!getCurrentRouteName().empty()) {
    m_journeyMap[getCurrentRouteName()].setGuidanceMeasurementSystem(m_unitsOfMeasurement);
  }
}

// Speak close range guidance instruction
void navcoreRoute::speakCloseInstruction(unsigned int index)
{
  if (!m_currentJourney.empty()) {
    m_journeyMap[m_currentJourney].speakCloseInstruction(index);
  }
}

// The speed of simulation in metres per second
void navcoreRoute::setSimulatorSpeed(unsigned int speed)
{
  m_simulatorSpeed = speed;

  if (m_isSimulating) {
    simulator_setSpeedLimit(SPEED_LIMIT_MOTORWAY, m_simulatorSpeed * 100);
  }
}

// Use this to speed up, freeze, reverse(!) simulator logical time
void navcoreRoute::setSimulatorRate(int logicalTimeScale)
{
  m_simulatorRate = logicalTimeScale;
}

// Retrieves the speed of the simulator in metres per second
unsigned int navcoreRoute::getSimulatorSpeed(void)
{
  return m_simulatorSpeed;
}

// Retrieve the logical time scaling factor
int navcoreRoute::getSimulatorRate(void)
{
  return m_simulatorRate;
}

// The current simulation state
bool navcoreRoute::isSimulating(void)
{
  return m_isSimulating;
}

// Are we there yet?
bool navcoreRoute::isSimulationFinished(void)
{
  return m_isSimulationFinished;
}

// Position of car in the driving simulation
void navcoreRoute::startDriving(const TPos& carPos)
{
  if (!m_currentJourney.empty()) {
    gps_setUTC(os_time());

    m_journeyMap[m_currentJourney].setAsStart(carPos);
    m_journeyMap[m_currentJourney].setMaplock();
    m_journeyMap[m_currentJourney].updateMaplock();

    if (m_journeyMap[m_currentJourney].calculateJourney()) {
      if (m_journeyMap[m_currentJourney].calculateGuidance()) {
        maplock_reset();

        gps_setContext(NMEA_CONTEXT_MANUAL);
      } else {
        LOG_WARN(0, ("Could not recalculate guidance list in driving simulation"));
      }
    } else {
      LOG_WARN(0, ("Could not recalculate journey in driving simulation"));
    }
  }
}

// Start auto-simulating the current route
void navcoreRoute::startSimulating(void)
{
  //DEBUG_DRAW_CLEAR();
  if (m_tempJourney != NULL) {
    m_isSimulating = true;
    m_isSimulationFinished = false;

    m_tempJourney->resetGuidance();

    maplock_reset();
    m_tempJourney->updateMaplock();
    journeySimulator_reset(m_tempJourney->getJourney());
    journeySimulator_resetGuide(m_tempJourney->getGuidanceList());
    simulator_setSpeedLimit(SPEED_LIMIT_MOTORWAY, getSimulatorSpeed() * 100);
    gps_setContext(NMEA_CONTEXT_SIMULATOR);
  }
}

// Progress the simulation
void navcoreRoute::simulateRoute(void)
{
  if (m_tempJourney != NULL) {

    m_tempJourney->updateMaplock();

    // Advance things along by 1 second.  By default this should result in
    // "real" speeds.
    if (simulator_updateMS(1000) == FALSE) {
      m_isSimulationFinished = true;
    }

    m_tempJourney->updateGuidance();
  }
}

void navcoreRoute::startmanualSim()
{
    maplock_reset();
    updateMapLock();
    m_ismanualSimulating = true;
    m_ismanualSimulateFinished = false;
    gps_setContext(NMEA_CONTEXT_MANUAL);
}

// Handle moving the car around
void navcoreRoute::driveCar(const TPos& carPos)
{
  if (m_tempJourney!=NULL) {
      manualSimulate(carPos,
                     maplock_getHeading(),
                     maplock_getSpeed(),
//                     gps_getUTC());
      os_time());
  }
}

// Used for driving simulator
void navcoreRoute::manualSimulate(const TPos& carPos, INT32 heading, INT32 speed, UINT32 utc)
{
  if (m_tempJourney!=NULL) {
    gps_setLatitude(carPos.y);
    gps_setLongitude(carPos.x);
    gps_setSpeed(speed);
    gps_setUTC(utc);
    gps_setHeading(heading);

    m_tempJourney->updateMaplock();
    BOOL8 onRoute = maplock_isOnRoute();
    if (onRoute) {
      m_timeOffRoute = gps_getUTC();
    }

    TInstructionReturnCode success= m_tempJourney->updateGuidance();

    // Heuristic for re-route
    if (!onRoute && (gps_getUTC() - m_timeOffRoute) > 1000)
    {
  //  if(!onRoute){
      LOG_DEBUG(0, ("Performing re-route"));
      m_tempJourney->recalculateJourney(carPos);
      m_tempJourney->calculateGuidance();
      m_timeOffRoute = gps_getUTC();
    }
  }
}

// Terminate the simulation
void navcoreRoute::stopmanualSimulating(void)
{
  m_ismanualSimulating = false;
}

// Terminate the simulation
void navcoreRoute::stopSimulating(void)
{
  m_isSimulating = false;
}

// Recalculate the currently selected route.  This method is useful for re-rendering a route "damaged" by driving.
bool navcoreRoute::recalculateRoute(void)
{
  //if (!m_currentJourney.empty()) {

  if (m_tempJourney != NULL) {
    m_tempJourney->calculateJourney();
    m_tempJourney->calculateGuidance();

    return true;
  } else {
    LOG_ERROR(0, ("Cannot re-calculate journey.  No active selected journey to re-calculate"));

    return false;
  }
}

// Re-routes the currently selected route using the current simulation position.
// Similar to the re-routing in SmartST.
bool navcoreRoute::reroute()
{
    if(m_tempJourney != NULL)
    {
        m_tempJourney->rerouteJourney();
        return true;
    }
    else
        return false;
}


// The speed of the car in km/h when the simulation is in progress
int navcoreRoute::getCurrentSpeedKPH(void)
{
  int speed;

  speed = gps_getKPH();

  return speed;
}

// The current heading when the simulation is in progress
float navcoreRoute::getCurrentHeading(void)
{
  float heading;

  heading = (float)gps_getHeading();
  heading /= 10000;

  return heading;
}

// The current position where the simulator is located when the simulation is in progress
void navcoreRoute::getCurrentLatLong(float& latitude, float& longitude)
{
  latitude = (float)gps_getLatitude() / 100000;
  longitude = (float)gps_getLongitude() / 100000;
}

//@TPE
void navcoreRoute::setTokenListDumpFile(BOOL8 bEnableDump)
{
    if(m_journeyMap.size() > 0)
    {
        if(bEnableDump > 0)
        {
            wstring szDumpFile = m_currentJourney;
			int nIdx = szDumpFile.rfind(L".");
			if(nIdx < 0)
			{
				szDumpFile += L".tkn";
			}
			else
			{
				szDumpFile.erase(nIdx);
				szDumpFile.append(L".tkn");
			}
			m_journeyMap[m_currentJourney].setTokenListDumpFile((wchar_t *)szDumpFile.c_str());
			return;
        }
    }
	m_journeyMap[m_currentJourney].setTokenListDumpFile(0);
}

void navcoreRoute :: destination_getMapLocation(TMapLocation *loc)
{
    this->m_tempJourney->getMapLoc(loc);
}

void navcoreRoute::deleteAvoidanceArea(TPathGeobanHandle handle)
{
  banAreas_deleteBan(handle);
}

TPathGeobanHandle navcoreRoute::addAvoidanceArea(const TAvoidanceArea *area)
{
  return banAreas_add(area);
}



