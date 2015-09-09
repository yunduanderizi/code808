#ifndef _NAVCOREFAVOURITES_H
#define _NAVCOREFAVOURITES_H

#pragma once
#include <vector>
#include "innovEngine_types.h"
#include "innovEngine_graphics.h"
#include "innovEngine_display.h"

class navcoreFavourites
{
public:
  navcoreFavourites(void);
  virtual ~navcoreFavourites(void);

  typedef enum GeocodeResultType {
    TYPE_PLACE,
    TYPE_POSTCODE,
    TYPE_ROAD,
    // TYPE_OTHER is used to account for entities that appear in the result
    // list of a geocode operation but are NOT geolocateable such as countries,
    // and states.
    TYPE_OTHER,
	TYPE_OTHER1,
	TYPE_OTHER2,
	TYPE_OTHER3,
	ICON_TYPE_SEARCH_RESULT
  } GeocodeResultType;

  // Add favourite to list of favourites to be displayed on the map.  Note that
  // not only is the "private" entity bounding box passed in but also the value 
  // returned from a geocode_locate* function.
  void add(GeocodeResultType type,
           const TRect * entityBound,
           const TPos *mapPos,
           const UINT32 size);
  // This marks a favourite for visual highlighting when selected
  void select(const unsigned int pos);
  // Deselect the currently selected favourite, nothing should be highlighted in this case
  bool deselect(const unsigned int pos);
  // Remove all current favourites
  void clearAll(void);
  // Render the list of favourites.  Any highlighted favourite will be rendered differently.  Currently only one favourite can be highlighted in a collection
  void render(TBitmap*myfavouriteicon, TViewport * viewport);

private:
  // -1 if no favourite selected, >= 0 if favourite is selected.
  int m_currentlySelectedFavourite;

  typedef struct EntityGeometry {
    GeocodeResultType type;
    TRect bBox;
    TPos pos;
    UINT32 size;
  } EntityGeometry;

  // The vector of favourites to be displayed from geosearch.
  std::vector<EntityGeometry> m_favouritesCollection;

  // For unselected entities.
  TBitmap *m_normalIcon;
  UINT8 m_normalIconImageData[10240];

  // To highlight a selected entity.
  TBitmap *m_selectedIcon;
  UINT8 m_selectedIconImageData[10240];

  // Geo-pens for rendering bounding boxes.
  RGBCOLOUR m_orange;
  RGBCOLOUR m_red;
  TPen m_placePen;
  TPen m_postCodePen;
  TPen m_roadPen;
  TPen m_highlightPen;
  TGeometricPen m_placeGeoPen;
  TGeometricPen m_postCodeGeoPen;
  TGeometricPen m_roadGeoPen;
  TGeometricPen m_highlightGeoPen;

  // Draw the bounding box
  void drawBBox(TViewport *viewport, TRect *bBox);
};

#endif //_NAVCOREFAVOURITES_H
