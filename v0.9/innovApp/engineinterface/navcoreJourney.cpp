#include "navcoreJourney.h"
#include <location/locmaplocationP.h>
#include <fstream>

#define LOGSCOPE LOG_APPLICATION

extern void TLanguageSetTokenListDump( wchar_t* pszFilename );

/*----------------------------------------------------------------*/
navcoreJourney::navcoreJourney(void)
: m_isRouteCalculated(false)
, m_isAReRoute(false)
, m_isValidIntersection(false)
, m_showOnMap(true)
, m_changedRenderColor(false)
{
  m_renderColor = RGBA_Define(192,64,64,0);

  // Route and itinerary related stuff
  m_journeyCalculatorResult = JOURNEYCALC_RESULT_READY_TO_START;
  itinerary_clear(&m_itinerary);
  m_routeBuffer = new UINT32[ROUTE_BUFFER_DWORDS];
  journey_initialise(&m_journey, m_routeBuffer, ROUTE_BUFFER_DWORDS);

  maplocation_init(&m_startLoc);

  // Guidance related stuff
  m_guideInstructionBuffer = new UINT8[GUIDE_BUFFER_SIZE];
  guideList_init(&m_guidanceList, m_guideInstructionBuffer, GUIDE_BUFFER_SIZE);
  guideContext_init(&m_guidanceContext);

#if testHighWayInfoAPI
  for (int i=0; i<ITINERARY_MAX_DESTINATIONS; i++)
  {
    m_hgihwayInfoBuffer[i] = new UINT8[1024*sizeof(THighWayInfo)];
    guideList_HighwayInfo_init(&this->m_highwayList, i, &this->m_hgihwayInfoBuffer[i], 1024*sizeof(THighWayInfo));
  }
#endif

  intersectionPic_init(&m_intersectionPictorial);
  m_unitsOfMeasurement = DIST_UNIT_METRIC;

  // Create enum to string maps
  initEnumToStringMaps();
}

/*----------------------------------------------------------------*/
navcoreJourney::~navcoreJourney(void)
{
  delete m_routeBuffer;
  delete m_guideInstructionBuffer;

#if testHighWayInfoAPI
  for (int i=0; i<ITINERARY_MAX_DESTINATIONS; i++)
  {
    delete m_hgihwayInfoBuffer[i];
  }
#endif

}

/*----------------------------------------------------------------*/
// Copy constructor
navcoreJourney::navcoreJourney(const navcoreJourney& copy)
{
  m_renderColor = copy.m_renderColor;
  m_changedRenderColor = copy.m_changedRenderColor;
  m_showOnMap = copy.m_showOnMap;
  m_journeyCalculatorResult = JOURNEYCALC_RESULT_READY_TO_START;
  itinerary_clear(&m_itinerary);
  m_routeBuffer = new UINT32[ROUTE_BUFFER_DWORDS];
  journey_initialise(&m_journey, m_routeBuffer, ROUTE_BUFFER_DWORDS);

  m_startLoc = copy.m_startLoc;
  m_reRouteLoc = copy.m_reRouteLoc;
  m_dest = copy.m_dest;
  m_journeyCalculatorResult = copy.m_journeyCalculatorResult;
  m_isAReRoute = copy.m_isAReRoute;
  m_isRouteCalculated = copy.m_isRouteCalculated;
  m_unitsOfMeasurement = copy.m_unitsOfMeasurement;

  itinerary_clone(&copy.m_itinerary, &m_itinerary);
  journey_clone(&copy.m_journey, &m_journey);

  // Guidance related stuff
  m_guideInstructionBuffer = new UINT8[GUIDE_BUFFER_SIZE];
  guideList_init(&m_guidanceList, m_guideInstructionBuffer, GUIDE_BUFFER_SIZE);
  guideContext_init(&m_guidanceContext);
  intersectionPic_init(&m_intersectionPictorial);

  // Recreate guidance list...
  if (m_isRouteCalculated)
    calculateGuidance();

  // Create enum to string maps
  initEnumToStringMaps();
}

/*----------------------------------------------------------------*/
// Assignment operator overload
// NB: I have this working with heap allocated objects but stack allocated
// objects are giving me grief here.  This has an impact on the
// navcoreMultiRouting::loadRoute() method.
navcoreJourney& navcoreJourney::operator=(navcoreJourney& rhs)
{
  if (this != &rhs) {
    m_renderColor = rhs.m_renderColor;
    m_changedRenderColor = rhs.m_changedRenderColor;
    m_showOnMap = rhs.m_showOnMap;
    m_startLoc = rhs.m_startLoc;
    m_reRouteLoc = rhs.m_reRouteLoc;
    m_dest = rhs.m_dest;
    m_journeyCalculatorResult = rhs.m_journeyCalculatorResult;
    m_isAReRoute = rhs.m_isAReRoute;
    m_isRouteCalculated = rhs.m_isRouteCalculated;
    m_unitsOfMeasurement = rhs.m_unitsOfMeasurement;

    itinerary_clone(&rhs.m_itinerary, &m_itinerary);
    journey_clone(&rhs.m_journey, &m_journey);

    // Recreate guidance list...
    calculateGuidance();
  }

  return *this;
}

/*----------------------------------------------------------------*/
// This sets the start of the route
bool navcoreJourney::setAsStart(const TPos& startPos)
{
  TMapLocation *pMapLoc = m_isAReRoute ? &m_reRouteLoc : &m_startLoc;

  maplocation_initUserLocation(pMapLoc, &startPos, NULL, -1, ROAD_TYPES );
  setAsStart(pMapLoc);
  return true;
}

bool navcoreJourney::setAsStart(TMapLocation * pMapLoc)
{
  TMapLocation *pTempLoc = m_isAReRoute ? &m_reRouteLoc : &m_startLoc;
  *pTempLoc = *pMapLoc;
  if(!maplocation_resolveOnNetwork(pTempLoc, NULL)){
    LOG_ERROR(0, ("Could not get aorigin network location from road location [%.5f, %.5f]",
      (float)pMapLoc->user_pos.y / 100000, (float)pMapLoc->user_pos.x / 100000));
    return false;
  }
  return true;
}

/*----------------------------------------------------------------*/
// This sets the end of the itinerary and any intermediate destinations along
// the journey
bool navcoreJourney::setAsDestination(const TPos& destPos,
                                      const TRoutingOptions& options)
{
  TMapLocation mapLoc;

  maplocation_initUserLocation(&mapLoc, &destPos, NULL, -1, ROAD_TYPES);

  return setAsDestination(&mapLoc, options);
}

bool navcoreJourney::setAsDestination(TMapLocation * pMapLoc, const TRoutingOptions &options)
{
  if (!maplocation_resolveOnNetwork(pMapLoc, NULL)) {
    LOG_ERROR(0, ("Could not get destination network location from map location [%.5f, %.5f]",
      (float)pMapLoc->user_pos.y / 100000, (float)pMapLoc->user_pos.x / 100000));
//    MessageBox(NULL,
//      _T("Could not set destination position(s)."),
//      _T("Destination was not a valid road."),
//      MB_OK);
    return false;
  }

  destination_init(&m_dest, pMapLoc, &options);

  if (!itinerary_addDestination(&m_itinerary, &m_dest)) {
//    MessageBox(NULL,
//      _T("Could not set destination position(s)."),
//      _T("Itinerary may be full."),
//      MB_OK);
    return false;
  }

  return true;
}

/*----------------------------------------------------------------*/
// Get the number of destinations in the itinerary
unsigned int navcoreJourney::getNumberOfItineraryDestinations(void)
{
  return itinerary_getDestinationCount(&m_itinerary);
}

/*----------------------------------------------------------------*/
// Get the number of legs in the journey
unsigned int navcoreJourney::getNumberOfJourneyLegs(void)
{
  return journey_getLegCount(&m_journey);
}

/*----------------------------------------------------------------*/
// Retrieve the start point of the journey
bool navcoreJourney::getStartPos(TPos& pos)
{
  if (!isValidItinerary()) {
    pos.y = pos.x = GPS_BADVALUE;
    return false;
  }

  return maplocation_getNetworkPos(&m_startLoc, &pos) == TRUE;
}

/*----------------------------------------------------------------*/
// Retrieve a position from the itinerary
bool navcoreJourney::getDestPos(TItineraryDestNumber destNum, TPos& pos)
{
  TItineraryDest dest;
  TMapLocation mapLoc;

  if (!isValidItinerary() || destNum >= getNumberOfItineraryDestinations()) {
    LOG_ERROR(0, ("Invalid itinerary index"));
    return false;
  }

  itinerary_getDestination(&m_itinerary, destNum, &dest);

  destination_getMapLocation(&dest, &mapLoc);

  return maplocation_getNetworkPos(&mapLoc, &pos) == TRUE;

}

/*----------------------------------------------------------------*/
// Get the start address of the journey
void navcoreJourney::getStartAddress(UTF16 * startAddress)
{
  startAddress[0] = 0;

  //FIXME This needs to be fixed once the new reverse geocode api has been set
  maplocation_getNetworkRoadName(&m_startLoc, startAddress, GEOSEARCH_MAX_NAME_SIZE);
#if 0
  if (!networkLocation_convertToRoadLocation(&m_startLoc, &roadLoc) ||
      !geosearch_initMapLocationFromRoadLocation(&mapLoc, &roadLoc, ROAD_TYPES, 100) ||
      !geosearch_getMapLocationDisplayName(&mapLoc, startAddress, 256)) {
    return;
  }
#endif
}

/*----------------------------------------------------------------*/
// Return the name of the road associated with this point in the itinerary
void navcoreJourney::getDestAddress(TItineraryDestNumber destNum, UTF16 * destAddress)
{
  TItineraryDest dest;
  TMapLocation mapLoc;

  destAddress[0] = 0;

  itinerary_getDestination(&m_itinerary, destNum, &dest);
  destination_getMapLocation(&dest, &mapLoc);
  //FIXME This needs to be fixed once the new reverse geocode api has been set
  maplocation_getNetworkRoadName(&mapLoc, destAddress, GEOSEARCH_MAX_NAME_SIZE);
 #if 0
  if (!itinerary_getDestination(&m_itinerary, destNum, &dest) ||
      !networkLocation_convertToRoadLocation(&dest.networkLocation, &roadLoc) ||
      !geosearch_initMapLocationFromRoadLocation(&mapLoc, &roadLoc, ROAD_TYPES, 100) ||
      !geosearch_getMapLocationDisplayName(&mapLoc, destAddress, 256)) {
    return;
  }
 #endif
}

/*----------------------------------------------------------------*/
// Determine whether a valid itinerary exists for this journey
bool navcoreJourney::isValidItinerary(void)
{
  TMapLocation dest;

  destination_getMapLocation(&m_dest, &dest);

  if ( maplocation_isValid(&m_startLoc) &&
       maplocation_isValid(&dest) ){
    return true;
  }

  return false;
}

/*----------------------------------------------------------------*/
// To handle when we go off route
void navcoreJourney::setReRoute(bool reRouting)
{
  m_isAReRoute = reRouting;
}

/*----------------------------------------------------------------*/
// Calculate the journey associated with the itinerary
bool navcoreJourney::calculateJourney(void)
{
  if (m_isAReRoute) {
    apiJourneyCalculator_reRouteTypeInitialise(&m_reRouteLoc,
                                               gps_getKPH(),
                                               TRUE,
                                               &m_journey);
  } else {
    if (isValidItinerary()) {
      m_journeyCalculatorResult =
        apiJourneyCalculator_initialise(&m_startLoc, 0, &m_itinerary, &m_journey);
    } else {
//      MessageBox(NULL,
//                 _T("Invalid itinerary:"),
//                 _T("start or destination points not set"),
//                 MB_OK);

      return false;
    }
  }

  do {
    m_journeyCalculatorResult = apiJourneyCalculator_iterate(500);
  } while (m_journeyCalculatorResult >= JOURNEYCALC_RESULT_READY_TO_START &&
           m_journeyCalculatorResult < JOURNEYCALC_RESULT_FINISHED);

  if (m_journeyCalculatorResult != JOURNEYCALC_RESULT_FINISHED) {
//    CString msg;
//    msg.Format(L"Error calculating route.  Got error %i", m_journeyCalculatorResult);
//    MessageBox(NULL, msg, _T("Routing Error:"), MB_OK);

    return false;
  }

  m_isRouteCalculated = true;

  return true;
}

/*----------------------------------------------------------------*/
// Recalculate the route subject to going off track
bool navcoreJourney::recalculateJourney(const TPos& newPos)
{
  bool rc;

  setReRoute(true);
  setAsStart(newPos);

  rc = calculateJourney();

  setReRoute(false);

  return rc;
}

/*----------------------------------------------------------------*/
// Calculate the guidance list for calculated journey
bool navcoreJourney::calculateGuidance(void)
{
  if (m_isRouteCalculated) { // Can only calculate a guidance list for a calculated journey.
    guideList_reset(&m_guidanceList, &m_journey);
    guideContext_reset(&m_guidanceContext, &m_guidanceList);

#if testHighWayInfoAPI
    Make_Highway_Info(&m_guidanceList, &m_highwayList);
#endif

    return true;
  } else {
    LOG_ERROR(0, ("Cannot render guidance list for uncalculated journey"));

    return false;
  }
}


/*----------------------------------------------------------------*/
// Re-routes journey using the current simulation position - Similar to the 
// re-routing in SmartST.
bool navcoreJourney::rerouteJourney()
{
  INT32             speed;

  // get current position, speed and heading from maplock
  updateMaplock();
  if (!maplock_isValid()) {
    //MessageBox(NULL, _T("Routing Error:"), L"Could not maplock current journey.", MB_OK);
    return false;
  }

  maplock_getMapLocation(&m_reRouteLoc);
  speed = maplock_getSpeed();

  // now initialize path calculator for re-routing ...
  apiJourneyCalculator_reRouteTypeInitialise(&m_reRouteLoc, speed, TRUE, &m_journey);

  // ... and do the actual re-routing
  do {
    m_journeyCalculatorResult = apiJourneyCalculator_iterate(500);
  } while (m_journeyCalculatorResult >= JOURNEYCALC_RESULT_READY_TO_START &&
           m_journeyCalculatorResult < JOURNEYCALC_RESULT_FINISHED);
  if (m_journeyCalculatorResult != JOURNEYCALC_RESULT_FINISHED) {
//    CString msg;
//    msg.Format(L"Error calculating route.  Got error %i", m_journeyCalculatorResult);
//    MessageBox(NULL, _T("Routing Error:"), msg, MB_OK);
    return false;
  }
  m_isRouteCalculated = true;

  // update guidance
  if (!navcoreJourney::calculateGuidance()) {
    return false;
  }

  // update simulator
  journeySimulator_reset(&m_journey);

  return true;
}


/*----------------------------------------------------------------*/
// Hint to map lock that we should stick to this journey.  Applies when we're simulating.
void navcoreJourney::setMaplock(void)
{
  // During simulations or "driving" we subsequently make calls to
  // maplock_updateJourney(&m_journey);
  maplock_setJourney(&m_journey);
}

/*----------------------------------------------------------------*/
// Update maplock for the journey
void navcoreJourney::updateMaplock(void)
{
  maplock_setJourney(&m_journey);
  maplock_updateJourney(&m_journey);
}

/*----------------------------------------------------------------*/
// Retrieve the "global" routing options associated with this route
bool navcoreJourney::getRoutingOptions(TItineraryDestNumber destNum,
                                       TRoutingOptions& options)
{
  TItineraryDest *pDest;

  if (destNum >= itinerary_getDestinationCount(&m_itinerary)) {
    return false;
  }

  pDest = &m_itinerary.destination[destNum];
  destination_getRoutingOptions(pDest, &options);

  return true;
}

/*----------------------------------------------------------------*/
// Set the "global" routing options for this journey
bool navcoreJourney::setRoutingOptions(TItineraryDestNumber destNum,
                                       const TRoutingOptions& options)
{
  TItineraryDest *pDest;

  if (destNum >= itinerary_getDestinationCount(&m_itinerary)) {
    return false;
  }

//  pDest = &m_itinerary.destination[destNum];
  pDest = &m_dest;
  destination_setRoutingOptions(pDest, &options);

  return true;
}

/*----------------------------------------------------------------*/
// Return the journey (for possible rendering, guidance or whatever...)
TJourney * navcoreJourney::getJourney(void)
{
  return &m_journey;
}

/*----------------------------------------------------------------*/
// Retrieve the guidance context associated with the route
TGuideContext * navcoreJourney::getGuideContext(void)
{
  return &m_guidanceContext;
}

/*----------------------------------------------------------------*/
// Write an XML routing test case for use in the ATF
void navcoreJourney::writeXMLRoutingTestCase(const char * filename)
{
  // Open the XML file and write the routing test case out.
  // Annotate with as much comment as possible.
  fstream routeTestCaseFile;
  TMapLocation mapLoc;
  TPos pos;

  routeTestCaseFile.open(filename, ios_base::out | ios_base::trunc);

  // Write XML prologue
  routeTestCaseFile << "<?xml version=\"1.0\"?>" << endl;
  routeTestCaseFile << "<!DOCTYPE mapcoretestsuite SYSTEM \"testdataformat.dtd\">" << endl;

  // Write <mapcoretestsuite> prologue
  routeTestCaseFile << "<mapcoretestsuite>" << endl;

  // Write <routingtestsuite> prologue
//  routeTestCaseFile << "   <routingtestsuite testsuitename=\"Generated by navtest\" " <<
//    "mapdatafilename=\"" << g_navMapCfg->GetMapConfigurationFilename() << "\">" << endl;

  // Write <routetestcase> prologue
  routeTestCaseFile << "      <routetestcase testCaseID=\"" << filename << "\">" << endl;

  // Write the relevant parts of the test case detail out...
  // According to the DTD:
  // 1. A setLocation tag MUST come first.
  // 2. An arbitrary list of addDestination tags may follow.
  maplocation_getNetworkPos(&m_startLoc, &pos);
  routeTestCaseFile << "         <setLocation latitude=\""
    << pos.y << "\" "
    << "longitude=\""
    << pos.x << "\"/>" << endl;

  for (unsigned int i = 0; i != itinerary_getDestinationCount(&m_itinerary); i++) {
    TItineraryDest dest;
    TRoutingOptions options;
    TPos pos;

    itinerary_getDestination(&m_itinerary, (TItineraryDestNumber)i, &dest);
    destination_getRoutingOptions(&dest, &options);
    destination_getMapLocation(&dest, &mapLoc);
    maplocation_getNetworkPos(&mapLoc, &pos);

    routeTestCaseFile << "         <addDestination latitude=\"" << pos.y << "\" "
      << "longitude=\"" << pos.x << "\"/>" << endl;

    // 3. A setRoutingOptions tag specifies parameters that govern route calculation.
    routeTestCaseFile << "         <setRoutingOptions " <<
      "vehicleType=\"" << (unsigned int)options.vehicleType << "\" " <<
      "shortestPref=\"" << (unsigned int)options.shortestDistancePreference << "\" " <<
      "freewayPref=\"" << (unsigned int)options.freewayUsagePreference << "\" " <<
      "unsealedPref=\"" << (unsigned int)options.unsealedUsagePreference << "\" " <<
      "urbanPref=\"" << (unsigned int)options.urbanAvoidancePreference << "\" " <<
      "ferryPref=\"" << (unsigned int)options.ferryUsagePreference << "\" " <<
      "avoidTolls=\"" << ((options.avoidTollsFlag == FALSE) ? "false" : "true") << "\" " <<
      "avoidUnsealed=\"" << ((options.avoidUnsealedFlag == FALSE) ? "false" : "true") << "\" " <<
      "avoidFerries=\"" << ((options.avoidFerriesFlag == FALSE) ? "false" : "true") << "\"/>" << endl;
  }

  // 4. A calculateRoute tag tells the test framework to calculate the route.
  routeTestCaseFile << "         <calculateRoute/>" << endl;

  // 5. A dumpInternalJourney tag dumps detail regarding the calculated journey.
  routeTestCaseFile << "         <dumpInternalJourney details=\"true\"/>" << endl;

  // Write </routetestcase> epilogue
  routeTestCaseFile << "      </routetestcase>" << endl;

  // Write </routingtestsuite> epilogue
  routeTestCaseFile << "   </routingtestsuite>" << endl;

  // Write </mapcoretestsuite> epilogue
  routeTestCaseFile << "</mapcoretestsuite>" << endl;

  routeTestCaseFile.close();
}

/*----------------------------------------------------------------*/
// Load a previously saved (and calculated) route
bool navcoreJourney::loadJourney(const char * filename)
{
  TPathRestartErrType result;
  OS_FILE loadFile;
  TJourneyLegNumber legNum, legCount;
  const TJourneyLeg *pLeg;
  const TPath *pPath;
  TItineraryDest dest;

  itinerary_clear(&m_itinerary);
  m_isRouteCalculated = false;

  // Open the file for reading (binary)...
  loadFile = os_fopen(filename, "rb");

  if (loadFile == NULL) {
    LOG_ERROR(0, ("Invalid file handle for file '%s'", filename));
    return false;
  }

  // Load the journey...
  result = journey_load(&m_journey, loadFile, NULL, NULL, NULL);
  os_fclose(loadFile);

  // If things loaded okay then make sure that the state reflects this.
  if (result != PATH_RESTART_SUCCESS ||
      !journey_getLeg(&m_journey, 0, &pLeg) ||
      !journeyLeg_getOrigin(pLeg, &m_startLoc)) {
    LOG_ERROR(0, ("Could not reload previously saved journey, error = %u", result));
    return false;
  }

  m_isRouteCalculated = true;

  // Now have to derive an itinerary from the loaded journey...
  legCount = journey_getLegCount(&m_journey);

  for (legNum = 0; legNum < legCount; legNum++) {
    if (!journey_getLeg(&m_journey, legNum, &pLeg) ||
        !journeyLeg_getPath(pLeg, &pPath)) {
      LOG_ERROR(0, ("Could not retrieve journey leg %u", legNum));
      break;
    }

    destination_init(&dest, &pLeg->origLoc, &pPath->options);

    itinerary_addDestination(&m_itinerary, &dest);
    m_dest = dest;
  }

  // Calculate a guidance list for the loaded journey...
  bool rc = calculateGuidance();

  return rc;
}

/*----------------------------------------------------------------*/
// Save this journey using the Map Core persistent journey API
bool navcoreJourney::saveJourney(const char * filename)
{
  TPathRestartErrType result;
  OS_FILE saveFile;

  // Save the journey if valid.
  if (m_isRouteCalculated && isValidItinerary()) {
    // Open the file for writing (binary)...
    saveFile = os_fopen(filename, "wb");

    // The actual save...
    if (saveFile) {
      result = journey_save(&m_journey, saveFile, NULL, NULL, NULL);
      os_fclose(saveFile);

      if (result == PATH_RESTART_SUCCESS) {
        return true;
      } else {
        LOG_ERROR(0, ("Could not save journey, error = %u", result));

        return false;
      }
    } else {
      LOG_ERROR(0, ("Could not open '%s' journey save file for writing", filename));

      return false;
    }
  } else {
    LOG_ERROR(0, ("Cannot save journey - invalid calculated route or itinerary"));

    return false;
  }
}

/*----------------------------------------------------------------*/
// The travel time in seconds for the whole journey
unsigned int navcoreJourney::getTravelTime(void)
{
  unsigned int legCount = getNumberOfJourneyLegs();
  int start;
  int end;
  unsigned int travelTime = 0;

  for (unsigned int i = 0; i < legCount; i++) {
    if (i == 0) {
      start = -1;
    } else {
      start = guideList_lastIndexForLeg(&m_guidanceList, i - 1);
    }
    end = guideList_lastIndexForLeg(&m_guidanceList, i);

    travelTime += guideList_timeBetweenInstr(&m_guidanceList, start, end);
  }

  return travelTime;
}

/*----------------------------------------------------------------*/
// The number of turns for the whole journey
unsigned int navcoreJourney::getNumberOfTurns(void)
{
  unsigned int legCount = getNumberOfJourneyLegs();
  unsigned int turnCount = 0;
  int start;
  int end;

  for (unsigned int i = 0; i < legCount; i++) {
    start = guideList_firstIndexForLeg(&m_guidanceList, i);
    end = guideList_lastIndexForLeg(&m_guidanceList, i);

    if (start != -1 && end != -1) {
      turnCount += (end - start);
    }
  }

  return turnCount;
}

/*----------------------------------------------------------------*/
// The number of instructions for the whole journey
unsigned int navcoreJourney::getNumberOfInstructions(void)
{
  return guideList_size(&m_guidanceList);
}

/*----------------------------------------------------------------*/
// The length of the whole journey
unsigned int navcoreJourney::getRouteLength(void)
{
  unsigned int legCount = getNumberOfJourneyLegs();
  unsigned int distance = 0;
  int start;
  int end;

  for (unsigned int i = 0; i < legCount; i++) {
    if (i == 0) {
      start = -1;
    } else {
      start = guideList_lastIndexForLeg(&m_guidanceList, i - 1);
    }
    end = guideList_lastIndexForLeg(&m_guidanceList, i);

    distance += guideList_distanceBetweenInstr(&m_guidanceList, start, end);
  }

  return distance;
}

/*----------------------------------------------------------------*/
// Reset guidance
void navcoreJourney::resetGuidance(void)
{
  guideContext_reset(&m_guidanceContext, &m_guidanceList);
}

/*----------------------------------------------------------------*/
// Update guidance for this journey and issue any instructions as required
TInstructionReturnCode navcoreJourney::updateGuidance(void)
{
  return guideContext_update(&m_guidanceContext);
}

/*----------------------------------------------------------------*/
// The location at which this guidance is issued
bool navcoreJourney::getGuidanceLocation(const unsigned int index, TPos& mapPos)
{
  TGuideInstruction *gdInstr;

  if (index < (UINT32)guideList_size(&m_guidanceList)) {
    gdInstr = guideList_get(&m_guidanceList, index);
    guideInstr_getPosition(gdInstr, &mapPos);

    return true;
  } else {
    mapPos.x = GPS_BADVALUE;
    mapPos.y = GPS_BADVALUE;

    return false;
  }
}

/*----------------------------------------------------------------*/
bool navcoreJourney::getGuidanceSourceHeading(unsigned int index, UINT8& heading)
{
  TGuideInstruction *gdInstr;

  if (index < (UINT32)guideList_size(&m_guidanceList)) {
    gdInstr = guideList_get(&m_guidanceList, index);
    heading = guideInstr_getSourceHeading(gdInstr);

    return true;
  } else {
    heading = 0;

    return false;
  }
}

/*----------------------------------------------------------------*/
// Initialise the intersection pictorial at this guidance point.
bool navcoreJourney::initialiseIntersectionPictorial(int index)
{
  m_isValidIntersection = false;
  memset(&m_intersectionDescriptor, 0, sizeof(TIntersectionDescr));

  if (index < guideList_size(&m_guidanceList)) {
    intersectionPic_init(&m_intersectionPictorial);

    INT32 rc = intersectionPic_setGuideIndex(&m_intersectionPictorial, &m_guidanceList, index);
    if (rc == 0) {
      // it's a valid intersection.
      m_isValidIntersection = true;

      // Now just fill in the attributes of the pictorial:
      // 1. Is it a roundabout?
      // 2. How many exits from the intersection/roundabout?
      // 3. What are the headings of each of these exits?
      // 4. Does the intersection exist in a lefthand/righthand road network?  This is
      //    relevant for roundabout pictorial rendering.

      // fetch the exit number
      m_intersectionDescriptor.exitIndex = intersectionPic_getExitIndex(&m_intersectionPictorial, 0);

      // fetch the road handedness
      if (intersectionPic_getRoadHandedness(&m_intersectionPictorial) == ROAD_HANDEDNESS_LEFT) {
        m_intersectionDescriptor.isLeftHandDrive = true;
      } else {
        m_intersectionDescriptor.isLeftHandDrive = false;
      }

      // check if it's a roundabout
      if (intersectionPic_isRoundabout(&m_intersectionPictorial, 0)) {
        m_intersectionDescriptor.isRoundabout = true;
      } else {
        m_intersectionDescriptor.isRoundabout = false;
      }

      // fetch headings of exit segments
      m_intersectionDescriptor.roadCount = intersectionPic_getRoadCount(&m_intersectionPictorial, 0);
      for (unsigned int i = 0; i < m_intersectionDescriptor.roadCount; i++) {
        m_intersectionDescriptor.exitHeadings[i] =
          intersectionPic_getRoadHeading(&m_intersectionPictorial, i, 0) * (float)360.0 / (float)256;
      }

      // fetch the lane pictorial data (if available)
      m_intersectionDescriptor.lanePicValid = false;
      if ( intersectionPic_hasLanePictorial( &m_intersectionPictorial ) &&
           intersectionPic_getLanePictorial( &m_intersectionPictorial, 
                                             &(m_intersectionDescriptor.lanePic) ) )
      {
        m_intersectionDescriptor.lanePicValid = true;
      }
    } else {
      // Check if the instruction represents an intermediate or final destination...
      TGuideInstruction *currentInstruction;

      currentInstruction = guideList_get(&m_guidanceList, index);
      if (guideInstr_isEndOfLeg(currentInstruction)) {
        m_intersectionDescriptor.isImmediateDest = true;
      }

      if (guideInstr_isEndOfJourney(currentInstruction)) {
        m_intersectionDescriptor.isFinalDest = true;
      }

      // This is a valid intersection too.
      m_isValidIntersection = true;
    }

    // It's a real intersection/roundabout, intermediate destination or final destination.
    return true;
  } else {
    LOG_ERROR(0, ("Supplied guidance index greater than guidance list size"));

    return false;
  }
}

/*----------------------------------------------------------------*/
// Return intersection attributes.  This assumes that:
// a) A valid journey and guidance list exist AND
// b) That initialiseIntersectionPictorial() for given index in the
//    guidance list has been initialised.
// If neither of these conditions are met then return false.
bool navcoreJourney::retrieveIntersectionPictorialAttributes(TIntersectionDescr& intDescr)
{
  if (m_isValidIntersection) {
    intDescr = m_intersectionDescriptor;

    return true;
  } else {
    LOG_ERROR(0, ("Attempted to return intersection pictorial attributes for uninitialised "
                  "intersection."));

    return false;
  }
}

/*----------------------------------------------------------------*/
// Convert turn style enum to a string
const wstring& navcoreJourney::TGuideTurnStyleToString(const TGuideTurnStyle styleValue)
{
  return m_guideTurnStyleMap[styleValue];
}

/*----------------------------------------------------------------*/
// Convert turn grade to a string
const wstring& navcoreJourney::TGuideTurnGradeToString(const TGuideTurnGrade gradeValue)
{
  return m_guideTurnGradeMap[gradeValue];
}

/*----------------------------------------------------------------*/
// Convert turn direction to a string
const wstring& navcoreJourney::TGuideTurnDirectionToString(const TGuideTurnDirection directionValue)
{
  return m_guideTurnDirectionMap[directionValue];
}

/*----------------------------------------------------------------*/
// Create enum to string lookup tables
void navcoreJourney::initEnumToStringMaps(void)
{
  m_guideTurnDirectionMap.clear();
  m_guideTurnGradeMap.clear();
  m_guideTurnStyleMap.clear();

  m_guideTurnDirectionMap[TURNDIRECTION_LEFT]         = wstring(L"Left");
  m_guideTurnDirectionMap[TURNDIRECTION_STRAIGHT]     = wstring(L"Straight");
  m_guideTurnDirectionMap[TURNDIRECTION_RIGHT]        = wstring(L"Right");
  m_guideTurnDirectionMap[TURNDIRECTION_CURRENT_ROAD] = wstring(L"Current Road");

  m_guideTurnGradeMap[TURNGRADE_NO_TURN]        = wstring(L"No turn");
  m_guideTurnGradeMap[TURNGRADE_NORMAL]         = wstring(L"Normal (deprecated)");
  m_guideTurnGradeMap[TURNGRADE_SLIGHT]         = wstring(L"Slight turn");
  m_guideTurnGradeMap[TURNGRADE_SLIGHT_MEDIUM]  = wstring(L"Slight medium turn (deprecated)");
  m_guideTurnGradeMap[TURNGRADE_MEDIUM]         = wstring(L"Medium turn");
  m_guideTurnGradeMap[TURNGRADE_MEDIUM_SHARP]   = wstring(L"Medium sharp turn (deprecated)");
  m_guideTurnGradeMap[TURNGRADE_SHARP]          = wstring(L"Sharp turn");
  m_guideTurnGradeMap[TURNGRADE_MAX_COMPARABLE] = wstring(L"Turn grade max comparable");
  m_guideTurnGradeMap[TURNGRADE_BEAR]           = wstring(L"Bear (deprecated)");
  m_guideTurnGradeMap[TURNGRADE_CONTINUE]       = wstring(L"Continue");
  m_guideTurnGradeMap[TURNGRADE_KEEP]           = wstring(L"Keep");
  m_guideTurnGradeMap[TURNGRADE_MAX]            = wstring(L"Turn grade max");

  m_guideTurnStyleMap[TURNSTYLE_REMOVE_THIS] = wstring(L"REMOVE THIS");
  m_guideTurnStyleMap[TURNSTYLE_NO_TURN] = wstring(L"No turn");
  m_guideTurnStyleMap[TURNSTYLE_GRADED_TURN] = wstring(L"Graded turn");
  m_guideTurnStyleMap[TURNSTYLE_EXIT] = wstring(L"Exit");
  m_guideTurnStyleMap[TURNSTYLE_POINT_ROUNDABOUT] = wstring(L"Point roundabout (deprecated)");
  m_guideTurnStyleMap[TURNSTYLE_ENTERING_ROUNDABOUT] = wstring(L"Entering roundabout (deprecated)");
  m_guideTurnStyleMap[TURNSTYLE_EXITING_ROUNDABOUT] = wstring(L"Exiting roundabout (deprecated)");
  m_guideTurnStyleMap[TURNSTYLE_BIFURCATION] = wstring(L"Bifurcation (deprecated)");
  m_guideTurnStyleMap[TURNSTYLE_OFFRAMP_BIFURCATION] = wstring(L"Offramp bifurcation (deprecated)");
  m_guideTurnStyleMap[TURNSTYLE_OFFRAMP] = wstring(L"Offramp (deprecated)");
  m_guideTurnStyleMap[TURNSTYLE_UTURN] = wstring(L"U-turn");
  m_guideTurnStyleMap[TURNSTYLE_TERMINATION] = wstring(L"Termination");
  m_guideTurnStyleMap[TURNSTYLE_INTERMEDIATE_DESTINATION] = wstring(L"Intermediate destination");
  m_guideTurnStyleMap[TURNSTYLE_FINAL_DESTINATION] = wstring(L"Final destination");
  m_guideTurnStyleMap[TURNSTYLE_ATTRIBUTE_CHANGE] = wstring(L"Attribute change");
  m_guideTurnStyleMap[TURNSTYLE_MERGE] = wstring(L"Merge");
  m_guideTurnStyleMap[TURNSTYLE_CURVE] = wstring(L"Curve");
  m_guideTurnStyleMap[TURNSTYLE_LANE] = wstring(L"Lane");
  m_guideTurnStyleMap[TURNSTYLE_FOLLOW_SIGNS] = wstring(L"Follow signs");
  m_guideTurnStyleMap[TURNSTYLE_APPROACHING] = wstring(L"Approaching");
  m_guideTurnStyleMap[TURNSTYLE_JOIN] = wstring(L"Join");
  m_guideTurnStyleMap[TURNSTYLE_INITIAL] = wstring(L"Initial");
  m_guideTurnStyleMap[TURNSTYLE_MAX] = wstring(L"Turn style max");
}

/*----------------------------------------------------------------*/
// Retrieve all guidance details for the index in the guidance list
bool navcoreJourney::getGuidanceInstruction(const unsigned int guideIndex,
                                            wstring& distanceFromStart,
                                            wstring& instruction,
                                            wstring& grade,
                                            wstring& style)
{
  if (guideIndex < (UINT32)guideList_size(&m_guidanceList)) {
    UINT32 distance;
    wchar_t distanceCString[256];
    UTF16 instructionCString[256] = {0};
    TGuideInstruction *guideInstr;
    TLanguageInstruction langInstr;

    distance = guideList_distanceFromStart(&m_guidanceList, guideIndex);
    //_snwprintf(distanceCString, sizeof(distanceCString), L"%u", distance);
    distanceFromStart = distanceCString;

    guideInstr = guideList_get(&m_guidanceList, guideIndex);
    language_guideToLanguageInstruction(&langInstr, guideInstr);
    language_textInstruction(&langInstr, 1, instructionCString, 256);
    //instruction = instructionCString;
    //FU2WConvert(instructionCString,sizeof(instructionCString),instruction,256);
    grade = TGuideTurnGradeToString(guideInstr->grade);

    style = TGuideTurnStyleToString(guideInstr->style);

    return true;
  } else {
    LOG_ERROR(0, ("Invalid index for this guidance list"));

    return false;
  }
}

bool navcoreJourney::getGuidanceInfo(const unsigned int guideIndex,
                                            wstring& distanceFromStart,
                                            wstring& roadname,
                                            wstring& grade,
                                            wstring& style,
                                            TSegmentID &nsegmentid)
{
  if (guideIndex < (UINT32)guideList_size(&m_guidanceList)) {
    UINT32 distance;
    wchar_t distanceCString[256];
    UTF16 instructionCString[256] = {0};
    TGuideInstruction *guideInstr;
    TLanguageInstruction langInstr;

    distance = guideList_distanceFromStart(&m_guidanceList, guideIndex);
    //_snwprintf(distanceCString, sizeof(distanceCString), L"%u", distance);
    distanceFromStart = distanceCString;

    guideInstr = guideList_get(&m_guidanceList, guideIndex);
    language_guideToLanguageInstruction(&langInstr, guideInstr);

    roadname = (wchar_t*)langInstr.roadName;
    nsegmentid = guideInstr->entrySegment;
    //instruction = instructionCString;
    //FU2WConvert(instructionCString,sizeof(instructionCString),instruction,256);
    grade = TGuideTurnGradeToString(guideInstr->grade);

    style = TGuideTurnStyleToString(guideInstr->style);

    return true;
  } else {
    LOG_ERROR(0, ("Invalid index for this guidance list"));

    return false;
  }
}
/*----------------------------------------------------------------*/
// Return the actual TGuideInstruction rather than extracting selected pieces
TGuideInstruction * navcoreJourney::getGuidanceInstruction(const unsigned int guideIndex)
{
  if (guideIndex < (UINT32)guideList_size(&m_guidanceList)) {
    return guideList_get(&m_guidanceList, guideIndex);
  }

  LOG_ERROR(0, ("Invalid index for this guidance list"));
  return NULL;
}

TGuideList * navcoreJourney::getGuidanceList(void)
{
  return &m_guidanceList;
}

/*----------------------------------------------------------------*/
bool navcoreJourney::getGuidanceInstruction(const unsigned int guideIndex,
                                            UTF16* instructionText)
{
  if (guideIndex < (UINT32)guideList_size(&m_guidanceList)) {
    TGuideInstruction *guideInstr;
    TLanguageInstruction langInstr;
    UTF16 instructionCString[256];

    guideInstr = guideList_get(&m_guidanceList, guideIndex);
    language_guideToLanguageInstruction(&langInstr, guideInstr);
    language_textInstruction(&langInstr, 1, instructionCString, 256);
    //instructionText = instructionCString;
    memcpy(instructionText, instructionCString, sizeof(instructionCString));

    return true;
  } else {
    LOG_ERROR(0, ("Invalid index for this guidance list"));
    instructionText = (UTF16*)L"";

    return false;
  }
}

/*----------------------------------------------------------------*/
// Retrieve details of itinerary destinations (location and streetname)
bool navcoreJourney::getItineraryDestinationInfo(TItineraryDestNumber destNum,
                                                 wstring& location,
                                                 wstring& streetname)
{
  if (isValidItinerary() && destNum < getNumberOfJourneyLegs()) {
    TPos destPos;
    wchar_t loc[128];
    UTF16 street[256];

    getDestPos(destNum, destPos);
//    _snwprintf(loc,
//             sizeof(loc),
//             L"%.5f, %.5f",
//             (float)destPos.y / 100000,
//             (float)destPos.x / 100000);
    getDestAddress(destNum, street);

    location = loc;
    streetname = L"streetname";
//    streetname = street;

    return true;
  } else {
    LOG_ERROR(0, ("Invalid itinerary index"));

    location.clear();
    streetname.clear();

    return false;
  }
}

/*----------------------------------------------------------------*/
bool navcoreJourney::getItineraryDestinationLocation(TItineraryDestNumber destNum,
                                                     TPos& location)
{
  if (!isValidItinerary() || !getDestPos(destNum, location)) {
    LOG_ERROR(0, ("Invalid itinerary index"));
    location.x = GPS_BADVALUE;
    location.y = GPS_BADVALUE;
  }
  return true;
}

/*----------------------------------------------------------------*/
// Retrieve details of where the itinerary "starts" from (location and streetname)
bool navcoreJourney::getItineraryStartInfo(wstring& location, wstring& streetname)
{
  if (isValidItinerary()) {
    TPos startPos;
    wchar_t loc[128];
    UTF16 street[256];

    getStartPos(startPos);

//    _snwprintf(loc,
//             sizeof(loc),
//             L"%.5f, %.5f",
//             (float)startPos.y / 100000,
//             (float)startPos.x / 100000);
    getStartAddress(street);

    location = loc;
    streetname = L"streetname";
//    streetname = street;

    return true;
  } else {
    LOG_ERROR(0, ("Invalid itinerary"));

    location.clear();
    streetname.clear();

    return false;
  }
}

/*----------------------------------------------------------------*/
bool navcoreJourney::getItineraryStartLocation(TPos& location)
{
  if (isValidItinerary()) {
    TPos startPos;

    getStartPos(startPos);

    location.x = startPos.x;
    location.y = startPos.y;

    return true;
  } else {
    LOG_ERROR(0, ("Invalid itinerary"));

    location.x = GPS_BADVALUE;
    location.y = GPS_BADVALUE;

    return false;
  }
}

/*----------------------------------------------------------------*/
// Speak close range guidance instruction
void navcoreJourney::speakCloseInstruction(unsigned int index)
{
  TGuideInstruction *currentInstr;
  TGuideInstruction *nextInstr;
  TLanguageInstruction dummyLanguageInstruction;
  TLanguageInstruction currentLanguageInstruction;
  TLanguageInstruction nextLanguageInstruction;

  currentInstr = guideList_get(&m_guidanceList, index);
  if (index + 1 < (unsigned int)guideList_size(&m_guidanceList)) {
    nextInstr = guideList_get(&m_guidanceList, index);
    language_guideToLanguageInstruction(&nextLanguageInstruction, nextInstr);
    language_guideToLanguageInstruction(&currentLanguageInstruction, currentInstr);
  } else {
    //language_initInstruction(&dummyLanguageInstruction);
    language_guideToLanguageInstruction(&currentLanguageInstruction, currentInstr);
    nextLanguageInstruction = dummyLanguageInstruction;
  }

//  g_navSoundcfg->playCloseRangePrompt(currentLanguageInstruction.grade,
//    currentLanguageInstruction.direction,
//    currentLanguageInstruction.style,
//    0,
//    FALSE,
//    nextLanguageInstruction.grade,
//    nextLanguageInstruction.direction,
//    nextLanguageInstruction.style,
//    nextLanguageInstruction.flags,
//    0,
//    m_unitsOfMeasurement,
//    currentInstr->vehicle
//    );
}

/*----------------------------------------------------------------*/
// Set up the measurement system for this journey
void navcoreJourney::setGuidanceMeasurementSystem(const TDistanceCombination unitsOfMeasurement)
{
  BOOL8 rc;

  rc = guideContext_setDistanceUnits(&m_guidanceContext, unitsOfMeasurement);
  if (!rc) {
    LOG_ERROR(0, ("Current selected language does not support requested measurement system"));
  }
}

TMapLocation * navcoreJourney::GetStartPt()
{
  return m_isAReRoute ? &m_reRouteLoc : &m_startLoc;
}

void navcoreJourney::setTokenListDumpFile(wchar_t* pszFilename)
{
	TLanguageSetTokenListDump(pszFilename);
}

bool navcoreJourney::IfShowOnMap()
{
  return m_showOnMap;
}

void navcoreJourney::SetShowOnMap(bool showOnMap)
{
  m_showOnMap = showOnMap;
}

void navcoreJourney::SetRenderColor(RGBCOLOUR renderColor)
{
  m_renderColor = renderColor;
  m_changedRenderColor = true;
}

RGBCOLOUR navcoreJourney::GetRenderColor()
{
  return m_renderColor;
}

bool navcoreJourney::RenderColorChanged()
{
  return m_changedRenderColor;
}


void navcoreJourney ::getMapLoc(TMapLocation *loc)
{
    destination_getMapLocation(&m_dest, loc);
}
