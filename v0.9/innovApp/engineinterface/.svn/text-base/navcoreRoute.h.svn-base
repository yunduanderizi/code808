#ifndef NAVCOREROUTE_H
#define NAVCOREROUTE_H

#pragma once
#include <vector>
#include <string>
#include <map>
using namespace std;

// A forward declaration of TViewport used to be defined in enpigeo.h
#include <display/diviewport.h>

#include "navcoreJourney.h"


//
// The navcoreMultiRouting class contains state related to the management
// of journeys and also the simulation of those journeys.
//
// There may be many journeys but there is only ever ONE active simulation
// based on the currently selected route.  The member functions that deal
// with simulation and "driving" are associated with the currently "active"
// route.
//
class navcoreRoute;

class navcoreRoute
{
public:
  navcoreRoute(void);


  virtual ~navcoreRoute(void);

  //
  // Route management
  // ================
  // prefer to use TMapLocation, if it is null then use TPos instead
  // The protocol for defining a new route:
  // 1. startNewRoute() marks the location at which the first leg of the
  //    journey begins.
  // 2. addIntermediateToNewRoute() adds intermediate legs to a multi-stop
  //    journey.
  // 3. endNewRoute() marks the location at which the multi-stop journey
  //    ends.
  // There are two states for defining a journey; defining a journey
  // (m_definingNewJourney == true) and not defining a journey (!)
  // (m_definingNewJourney == false).
  bool startNewRoute(const TPos& mapPos, TMapLocation * pMapLoc);
  bool addIntermediateToNewRoute(const TPos& mapPos, const TRoutingOptions& options, TMapLocation * pMapLoc);
  bool endNewRoute(const TPos& mapPos, const TRoutingOptions& options, TMapLocation * pMapLoc);

  // Are we currently defining a new journey?
  bool isDefiningNewJourney(void);

  // This is the name of the currently selected (active) journey
  bool setCurrentRouteName(const wstring& selectedRouteName);
  // No current journey selected
  void unsetCurrentRouteName(void);
  // The currently selected "active" route
  const wstring& getCurrentRouteName(void);
  // The number of defined routes
  const unsigned int getNumberOfRoutes(void);
  // Retrieve the routing preferences of the currently selected route.
  bool getCurrentRouteNameRoutingPreferences(TRoutingOptions& options,
                                             TItineraryDestNumber destNum);

  // Set the routing preferences of the currently selected route.
  bool setCurrentRouteNameRoutingPreferences(const TRoutingOptions& options,
                                             TItineraryDestNumber destNum);

  // Return the n-th key (specified by index) of the container.  This is
  // really just a convenience for the UI to fill out a list box control.
  const vector<wstring>& getRouteNames(void);

  // Load a route from a file
  bool loadRoute(const wstring &pathName, const wstring& routeName);
  // Save the current selected route
  bool saveRoute(const wstring &pathName);
  // Save the currently selected route as an XML testcase
  bool saveXMLTestCase(const wstring& pathName);
  // Remove the currently selected route
  bool removeRoute(void);
  // Remove all routes
  bool removeAllRoutes(void);
  // Using the currently selected route, clone the journey giving it the name, newRouteName
  bool cloneRoute(const wstring& newRouteName);

  //
  // Route/Guidance Display
  // ======================
  //
  // Render any calculated routes
  void renderAllJourneys(TViewport * viewport);
  // Render any calculated journey flags
  void renderAllJourneyFlags(TViewport * viewport);
  // Render any guidance for calculated routes
  void renderAllGuidanceLists(TViewport * viewport);

  //
  // Guidance
  // ========
  //
  unsigned int getTravelTime(void);
  unsigned int getNumberOfTurns(void);
  unsigned int getNumberOfInstructions(void);
  unsigned int getRouteLength(void);
  unsigned int getNumberOfJourneyLegs(void);
  bool getGuidanceLocation(unsigned int index, TPos& mapPos);
  bool getGuidanceSourceHeading(unsigned int index, UINT8& heading);
  bool getGuidanceInstruction(const unsigned int guideIndex,
                              wstring& distanceFromStart,
                              wstring& instruction,
                              wstring& grade,
                              wstring& style);
  bool getGuidanceInfo(const unsigned int guideIndex,
                              wstring& distanceFromStart,
                              wstring& roadname,
                              wstring& grade,
                              wstring& style,
                              TSegmentID &nsegmentid);
  bool getGuidanceInstruction(const unsigned int guideIndex,
                              UTF16* instructionText);
  TGuideInstruction * getGuidanceInstruction(const unsigned int guideIndex);
  TGuideContext * getGuideContext(void);
  TGuideList * getGuideList(void);

  // Starting point of itinerary (not really a part of the itinerary as such)
  bool getItineraryStartInfo(wstring& location, wstring& streetname);
  bool getItineraryStartLocation(TPos& mapPos);

  // A intermediate destination (and final destination) on the journey
  bool getItineraryDestInfo(const unsigned int index, wstring& location, wstring& streetname);
  bool getItineraryDestLocation(TItineraryDestNumber destNum, TPos& mapPos);

  // Set the measurement system (variations on metric and imperial)
  void setGuidanceMeasureSystem(const TDistanceCombination unitsOfMeasurement);

  // Speak close range guidance instruction
  void speakCloseInstruction(unsigned int index);

  // Initialise the intersection pictorial
  bool initialiseIntersectionPictorial(unsigned int index);

  // Return intersection attributes.  This assumes that:
  // a) A valid journey and guidance list exist AND
  // b) That initialiseIntersectionPictorial() for given index in the
  //    guidance list has been initialised.
  // If neither of these conditions are met then return false.
  bool retrieveIntersectionPictorialAttributes(navcoreJourney::TIntersectionDescr& intDescr);

  //
  // Route/Simulator Information
  // ===========================
  //
  // The current simulation state
  bool isSimulating(void);

  //
  // Simulation and Driving
  // ======================
  //
  // The speed of simulation in metres per second
  void setSimulatorSpeed(unsigned int speed);

  // Use this to speed up, freeze, reverse(!) simulator logical time
  // +ive -> increase the rate of change
  // 0    -> freeze time
  // -ive -> reverse the rate of change
  void setSimulatorRate(int logicalTimeScale);

  // Retrieves the speed of the simulator in metres per second
  unsigned int getSimulatorSpeed(void);

  // Retrieve the logical time scaling factor
  int getSimulatorRate(void);

  // Position of car in the driving simulation
  void startDriving(const TPos& carPos);

  // Start auto-simulating the current route
  void startSimulating(void);

  // Terminate the simulation
  void stopSimulating(void);

  void stopmanualSimulating();
  // Progress the simulation
  void simulateRoute(void);

  // Handle moving the car around
  void driveCar(const TPos& carPos);

  void startmanualSim();
  // Used for driving simulator
  void manualSimulate(const TPos& carPos, INT32 heading, INT32 speed, UINT32 utc);

  // The speed of the car in km/h when the simulation is in progress
  int getCurrentSpeedKPH(void);

  // The current heading when the simulation is in progress
  float getCurrentHeading(void);

  // The current position where the simulator is located when the simulation is in progress
  void getCurrentLatLong(float& y, float& x);

  // Are we there yet?
  bool isSimulationFinished(void);

  // Recalculate the currently selected route.  This method is useful for
  // re-rendering a route "damaged" by driving.
  bool recalculateRoute(void);

  // Is the current journey name string non-empty?
  bool currentJourneyValid(void) { return m_tempJourney; }

  // Access current journey functions
  void updateMapLock(void) { m_tempJourney->updateMaplock();}
  TInstructionReturnCode updateGuidance(void) { return m_tempJourney->updateGuidance();}

  // Re-routes the currently selected route using the current simulation position.
  // Similar to the re-routing in SmartST.
  bool ismanulSimulating(void)  { return m_ismanualSimulating;}

  bool ismanulsimulationFinished(void) {return m_ismanualSimulateFinished;}

  bool reroute();

  bool GetCurrentJourney(navcoreJourney *pJourney);          // give the current journey, return false if cannot

  TPos * GetStartPoint();                                    // give the start point if has not set end point
  bool GetDestPoint(TItineraryDestNumber destNum, TPos& pos);                                    // give the start point if has not set end point
  map<wstring, navcoreJourney, less<wstring> > * GetJourneys();
  //@TPE
  void setTokenListDumpFile(BOOL8 bEnableDump);              // set the token list dump enable / disable.
  void setmanulsimulationfinished(bool type) {m_ismanualSimulateFinished = type;}


  void destination_getMapLocation(TMapLocation *loc);

  void deleteAvoidanceArea(TPathGeobanHandle handle);
  TPathGeobanHandle addAvoidanceArea(const TAvoidanceArea *area);
private:
  typedef map<wstring, navcoreJourney, less<wstring> > NamedJourney;

  // Maps a name to a navcoreJourney object
  NamedJourney m_journeyMap;
  vector<wstring> m_routeNames;

  // The name of the currently selected journey (if selected)
  wstring m_currentJourney;

  // The new journey being defined, but not yet calculated.
  wstring m_newJourney;

  // Used for loading and defining new journeys.
  navcoreJourney *m_tempJourney;

  // Used to flag whether we are currently defining a new journey.
  bool m_definingNewJourney;

  // Various choices of metric and imperial measurements for guidance
  TDistanceCombination m_unitsOfMeasurement;

  // Are we simulating the current route?
  bool m_isSimulating;

  // Are we there yet?
  bool m_isSimulationFinished;

  bool m_ismanualSimulating;

  bool m_ismanualSimulateFinished;

  // Speed of simulation in metres per second
  unsigned int m_simulatorSpeed;

  // Logical time scale factor
  int m_simulatorRate;

  // The time we have been off route while driving
  int m_timeOffRoute;


};

#endif //NAVCOREROUTE_H
