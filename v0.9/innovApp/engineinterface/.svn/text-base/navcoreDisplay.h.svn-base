#ifndef NAVCOREDISPLAY_H
#define NAVCOREDISPLAY_H

#include "navcore.h"
#include <innovEngine_sdk.h>
#include "system_param.h"

#define BITMAP_CACHE_BUFFER_SIZE (256 * 1024)

class navcoreDisplay : public navcore
{
public:
  navcoreDisplay();
  ~navcoreDisplay();
  void initDisplay(int screenWidth, int screenHeight);
  void refresh();

  bool zoomToPoint(int x, int y, double zoomFactor);

  //Edison add for locate cluster at 2008.9.3
  void zoomToPoint(TPos mapPos, double zoomFactor);

  void showCrossHairs(TPos crossHair);
  void showSegmentNodeID( TPos mapPos );
  void drawRuler();
  void changemapmode();
  void setmapmode(MAP_MODE mapmode);
  MAP_MODE getmapmode(){return m_mapmode;}

//  void changemapmode(int newMode);


  //pan map with left button
  void pan(TPos to,TPos from);
  //aim map (tilt and orient) with left+right button
  void aim(TPos to,TPos from);
  bool clickedOnMap(TPos screenPos);
  bool mousepress(int x, int y);
  void rememberCenter();
  bool mousemove(int x, int y);
  // Goto a map position.
  void gotoMap(TPos mapPos);
  // Goto a map position whose bounding box is constrained by size.  This
  // will set the map size if necessary.
  void gotoMap(TPos mapPos, unsigned int size);
  // Goto a map position whose bounds are defined as bounding box TRect.
  void gotoMap(TRect *bBox);
  void setDetailLevel(int detailLevel);

  //Rotate the display about the centre
  void rotateDisplay(int angle);
  void setDisplayHeading(UINT8 angle);

  //tilt the 3d display
  void set3dTilt(int angle);
//  void setRenderFlags( DEBUG_DISPLAY_FLAGS renderFlag );
//  DEBUG_DISPLAY_FLAGS getRenderFlags(  );

  //void setRoadLabeling( TJourney *pThis );

  int getCrossHairType(){return m_crossHairType;};
  void setCrossHairType(int type){m_crossHairType=type;};

  bool isInitialised( void );
  void setInitialised( bool flag ){ this->m_bDisplayInitialised = flag; };

  // Retrieve across all active maps loaded, a count of the number of
  // displayable POI categories.
  int getPOITypeCount( void );

  // Retrieve the POI category information for the nth POI in this index.
  //int getPOIName( TPOIDisplayType *poiCategoryName, int index );

  // Enable POI category for display
  int enableCategoryForDisplay( int typeId );

  // Disable POI category for display
  int disableCategoryForDisplay( int typeId );

  // Retrieve the current level of alpha transparency for POI display
  int getPOITransparency(void);

  // Set the level of alpha transparency for POI display
  bool setPOITransparency(int alphaLevel);

  //return the level of detail
  int getLevelOfDetail(void);

  // Retrieve the POI scale factor (between -2 and +2)
  int getPOIScaleFactor(void);

  // Set the POI icon scale factor.
  void setPOIScaleFactor(int scaleFactor);

  bool isPolyClassEnabled(TPolClass polyClass);

  void setPolygonClass(TPolClass polyClass, BOOL8 enable);

  bool isPatternedPolyEnabled(void);

  void setPatternedPolygon(BOOL8 enable);

  TElevationPalette * getElevationPalette( void ) {
    return Viewport_GetElevationPalette( &viewport );
  }

  void setCarIcon();
  void clearCarIcon();
  void setCarAngle(int heading);
  void setCarIconPos(TPos mapPos, UINT8 heading);
  bool onCarIcon(TPos screenPos, TPos carPos);
  TCarIcon *getCarIcon();
  TGeoban *getGeoban();
  TLatLongGrid *getLatLongGrid();
  TSky *getSky();

  // Returns TRUE if the snail trail is rendered, otherwise FALSE.
  BOOL8 getSnailTrailToggle() const;

  /// Enables/disables the snail trail toggle.
  /// @param[in]  on
  ///    Should be TRUe, if you want the snail trail being rendered.
  void setSnailTrailToggle( BOOL8 on );

  //resize the map display
  void resizeDisplay(int width, int height);

  // Get the font for a particular map display element.
  const TFont *getFont(TDisplayFontIndex index);
  // Set the font for a particular map display element.
  void setFont(TDisplayFontIndex index, const TFont *font);
  // Get the colour for a particular map legend display element.
  RGBCOLOUR getColour(TDisplayColours index);
  // Set the colour for the map display legend of a particular item.
  void setColour(TDisplayColours index, RGBCOLOUR colourVal);
  // Reset the map legend colour table to its default values
  void resetColour(void);
  // Reset the elevation palette to the default value in the current viewport
  void resetElevationPalette(void);
  BOOL8 isPolygonClassEnabled(TPolClass polyClass);
  RGBCOLOUR getPolygonFillColour(TPolClass polyClass);
  RGBCOLOUR getPolygonBorderColour(TPolClass polyClass);
  TPolKind getPolygonKind(TPolClass polyClass);
  const TBitmap * getPolygonBitmap(TPolClass polyClass);
  void setPolygonFillColour(TPolClass polyClass, RGBCOLOUR colour);
  void setTopoElevationGradients(TElevationPaletteEntry * pElevationTable, int size);

  // Return the current position of the mouse pointer on the map display (lat/long).
  TPos getCurrentMapPos(void)
  {
    return m_currentPos;
  }

  // Set the position in map co-ordinates at which the mouse pointer is located.
  void setCurrentMapPos(TPos mapPos)
  {
    m_currentPos = mapPos;
  }

  // Highlight the road located at this position
  void setHighlightRoadLocation(const TPos mapPos);
  // Clear highlight
  void resetHighLightRoadLocation(void);


private:

  bool m_bDisplayInitialised;
  TCarIcon m_carIcon; //to draw the car icon when routing
  TSky m_sky; //to draw the sky bitmap in the 3D display
  TDisplayNameSet m_displayNameSet;
  TGeoban m_geoban;

  int m_crossHairType;

  TLatLongGrid m_latLongGrid;
  UINT16 *m_pDecompBuffer;
  UINT32 m_cacheBuffer[( BITMAP_CACHE_BUFFER_SIZE + sizeof( UINT32 ) - 1 ) / sizeof( UINT32 )];

  // Where we currently are on the map
  TPos m_currentPos;
  BOOL8 m_renderSnailTrail;

  TPos m_initMouseMapPos;
  TPos m_xy;
  TPos m_center;
  MAP_MODE m_mapmode;
  BOOL8 m_mapangle_flag;


  bool zoom(double k);
  void initBuffer(int width, int height);
  void drawRulerPos(int size, int yPos);
  void setOrigin();
};

#endif // NAVCOREDISPLAY_H
