#ifndef NAVCOREJOURNEY_H
#define NAVCOREJOURNEY_H

#pragma once
#include <innovEngine_sdk.h>
#include <map>
#include <string>

using namespace std;

#define testHighWayInfoAPI 0

// The navcoreJourney class maintains the state of the journey as well as
// the guidance associated with the calculated journey.
class navcoreJourney
{
public:
  navcoreJourney(void);
  virtual ~navcoreJourney(void);

  // Copy constructor
  navcoreJourney(const navcoreJourney& copy);

  // Assignment operator overload
  navcoreJourney& operator=(navcoreJourney& rhs);

  // This sets the start of the route.
  bool setAsStart(const TPos& startPos);
  bool setAsStart(TMapLocation * pMapLoc);

  // This sets the end of the itinerary and any intermediate destinations
  // along the journey.
  bool setAsDestination(const TPos& destPos, const TRoutingOptions& options);
  bool setAsDestination(TMapLocation * pMapLoc, const TRoutingOptions& options);

  // Get the number of destinations in the itinerary
  unsigned int getNumberOfItineraryDestinations(void);

  // Get the number of legs in the journey
  unsigned int getNumberOfJourneyLegs(void);

  // Retrieve the start point of the journey
  bool getStartPos(TPos& pos);

  // Retrieve a position from the itinerary
  bool getDestPos(TItineraryDestNumber destNum, TPos& pos);

  // Get the start address of the journey
  void getStartAddress(UTF16 * address);

  void getMapLoc(TMapLocation *loc);

  // Return the name of the road associated with this point in the itinerary
  void getDestAddress(TItineraryDestNumber destNum, UTF16 * address);

  // Determine whether a valid itinerary exists for this journey
  bool isValidItinerary(void);

  // To handle when we go off route
  void setReRoute(bool reRouting);

  // Calculate the journey associated with the itinerary
  bool calculateJourney(void);

  // Recalculate the route subject to going off track
  bool recalculateJourney(const TPos& newPos);

  // Re-routes journey using the current simulation position - Similar to the 
  // re-routing in SmartST.
  bool rerouteJourney();

  // Hint to map lock that we should stick to this journey.  Applies mainly when we're simulating.
  void setMaplock(void);

  // Update maplock for the journey
  void updateMaplock(void);

  // Retrieve the "global" routing options associated with this route
  bool getRoutingOptions(TItineraryDestNumber destNum, TRoutingOptions& options);

  // Set the "global" routing options for this journey
  bool setRoutingOptions(TItineraryDestNumber destNum, const TRoutingOptions& options);

  // Return the journey (for possible rendering, guidance or whatever...)
  TJourney * getJourney(void);

  // Retrieve the guidance context associated with the route
  TGuideContext * getGuideContext(void);

  // Write an XML routing test case for use in the ATF
  void writeXMLRoutingTestCase(const char * filename);

  // Load a previously saved (and calculated) route
  bool loadJourney(const char * filename);

  // Save this journey using the Map Core persistent journey API
  bool saveJourney(const char * filename);

  // The travel time in seconds for the whole journey
  unsigned int getTravelTime(void);

  // The number of turns for the whole journey
  unsigned int getNumberOfTurns(void);

  // The number of instructions for the whole journey
  unsigned int getNumberOfInstructions(void);

  // The length of the whole journey
  unsigned int getRouteLength(void);

  // Calculate the guidance list for calculated journey
  bool calculateGuidance(void);

  // Reset guidance
  void resetGuidance(void);

  // Update guidance for this journey and issue any instructions as required
  TInstructionReturnCode updateGuidance(void);

  //@TPE 
  // set the token list output file name.
  void setTokenListDumpFile(wchar_t* pszFilename);

  // The location at which this guidance is issued
  bool getGuidanceLocation(const unsigned int index, TPos& mapPos);

  // The source heading in byte degrees of the incident entry point to
  // the intersection.
  bool getGuidanceSourceHeading(unsigned int index, UINT8& heading);

  // Initialise the intersection pictorial at this guidance point.
  bool initialiseIntersectionPictorial(int index);

  // A description of intersection attributes which can be used to render
  // an intersection pictorial.
  typedef struct TIntersectionDescr {
    // "Intersection" is immediate destination.
    bool                  isImmediateDest;
    // "Intersection" is final destination.
    bool                  isFinalDest;
    // Intersection is actually a roundabout.
    bool                  isRoundabout;
    // If a lefthand drive road network then true else false.
    bool                  isLeftHandDrive;
    // Number of roads exiting from an intersection.
    unsigned int          roadCount;
    // The index of the exit road.
    unsigned int          exitIndex;
    // The float array of exit headings from the intersection.
    float                 exitHeadings[32];
    // TRUE, if there is a valid lane pictorial
    bool                  lanePicValid;
    // The information about lanes
    TIntersectionLanePic  lanePic;
  } TIntersectionDescr;

  // Return intersection attributes.  This assumes that:
  // a) A valid journey and guidance list exist AND
  // b) That initialiseIntersectionPictorial() for given index in the
  //    guidance list has been initialised.
  // If neither of these conditions are met then return false.
  bool retrieveIntersectionPictorialAttributes(TIntersectionDescr& intDescr);

  // Retrieve all guidance details for the index in the guidance list
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
  // Just give me the actual instruction...
  TGuideInstruction * getGuidanceInstruction(const unsigned int guideIndex);
  TGuideList * getGuidanceList(void);

  // Retrieve details of where the itinerary "starts" from (location and streetname)
  bool getItineraryStartInfo(wstring& location, wstring& streetname);
  bool getItineraryStartLocation(TPos& location);

  // Retrieve details of itinerary destinations (location and streetname)
  bool getItineraryDestinationInfo(TItineraryDestNumber destNum,
                                   wstring& location,
                                   wstring& streetname);

  bool getItineraryDestinationLocation(TItineraryDestNumber destNum,
                                       TPos& location);

  // Speak close range guidance instruction
  void speakCloseInstruction(unsigned int index);

  // Set up the measurement system for this journey
  void setGuidanceMeasurementSystem(const TDistanceCombination unitsOfMeasurement);
  TMapLocation * GetStartPt();

  bool IfShowOnMap();

  void SetShowOnMap(bool showOnMap);

  void SetRenderColor(RGBCOLOUR renderColor);
  RGBCOLOUR GetRenderColor();
  bool RenderColorChanged();

private:
  static const int ROUTE_BUFFER_DWORDS = (24 * 1024);
  static const int GUIDE_BUFFER_SIZE = (64 * 1024);

  RGBCOLOUR m_renderColor;
  bool m_changedRenderColor;
  // Has the journey been calculated?
  bool m_isRouteCalculated;

  // Has this route gone off track?
  bool m_isAReRoute;

  bool m_showOnMap;                                          // if show on map or not

  // The state of journey calculation
  TJourneyCalculatorResult m_journeyCalculatorResult;

  // The journey and itinerary...
  TJourney             m_journey;
  TMapLocation         m_startLoc;
  TMapLocation         m_reRouteLoc;
  TItineraryDest       m_dest;
  TItinerary           m_itinerary;

  // 24KByte route buffer
  UINT32 *m_routeBuffer;

  // Guidance related objects...
  TGuideList           m_guidanceList;
  TGuideContext        m_guidanceContext;
  UINT8               *m_guideInstructionBuffer;
  TIntersectionPic     m_intersectionPictorial;
  bool                 m_isValidIntersection;
  TIntersectionDescr   m_intersectionDescriptor;
  TDistanceCombination m_unitsOfMeasurement;

  // Enum to string lookup tables
  map <TGuideTurnStyle, wstring> m_guideTurnStyleMap;
  map <TGuideTurnGrade, wstring> m_guideTurnGradeMap;
  map <TGuideTurnDirection, wstring> m_guideTurnDirectionMap;

#if testHighWayInfoAPI
  THighWayList  m_highwayList;
  UINT8        *m_hgihwayInfoBuffer[ITINERARY_MAX_DESTINATIONS];
#endif

  // Convert turn style enum to a string
  const wstring& TGuideTurnStyleToString(const TGuideTurnStyle styleValue);

  // Convert turn grade to a string
  const wstring& TGuideTurnGradeToString(const TGuideTurnGrade gradeValue);

  // Convert turn direction to a string
  const wstring& TGuideTurnDirectionToString(const TGuideTurnDirection directionValue);

  // Create enum to string lookup tables
  void initEnumToStringMaps(void);
};

#endif //NAVCOREJOURNEY_H
