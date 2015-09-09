#include "stdafx.h"

#include "navcoreFavourites.h"
#include "innovEngine_graphics.h"
#include "innovEngine_display.h"
#include "innovEngine_logging.h"
#include "interface_logistics.h"
// I need the following header because GeoPens appear not to be public.
#include "display/diviewportP.h"


navcoreFavourites::navcoreFavourites(void)
: m_currentlySelectedFavourite(-1)
{
  m_favouritesCollection.clear();

  // Load the standard imagery used for displaying favourites.
  m_normalIcon = Bitmap_LoadBmpFile("greenstar.bmp", m_normalIconImageData, 10240);
  if (m_normalIcon == NULL) {
    LOG_FATAL(0, ("Could not load normal icon"));
  }

  // Load the highlight imagery used for displaying favourites.
  m_selectedIcon = Bitmap_LoadBmpFile("goldstar.bmp", m_selectedIconImageData, 10240);
  if (m_selectedIcon == NULL) {
    LOG_FATAL(0, ("Could not load selected icon"));
  }

  m_orange.alpha = 0;
  m_orange.red = 255;
  m_orange.green = 128;
  m_orange.blue = 0;

  m_red.alpha = 0;
  m_red.red = 255;
  m_red.green = 0;
  m_red.blue = 0;

  Pen_New(&m_placePen);
  Pen_New(&m_postCodePen);
  Pen_New(&m_roadPen);
  Pen_New(&m_highlightPen);
  m_placePen.colour = m_orange;
  m_placePen.thickness = 0;
  m_placePen.alpha = 64;
  m_postCodePen.colour = m_orange;
  m_postCodePen.thickness = 0;
  m_postCodePen.alpha = 64;
  m_roadPen.colour = m_orange;
  m_roadPen.thickness = 0;
  m_roadPen.alpha = 64;
  m_highlightPen.colour = m_red;
  m_highlightPen.thickness = 1;
  m_highlightPen.alpha = 0;

  GeoPen_New(&m_placeGeoPen, &m_placePen, 0);
  GeoPen_New(&m_postCodeGeoPen, &m_postCodePen, 0);
  GeoPen_New(&m_roadGeoPen, &m_roadPen, 0);
  GeoPen_New(&m_highlightGeoPen, &m_highlightPen, 0);
}

navcoreFavourites::~navcoreFavourites(void)
{
}

// Add favourite to list of favourites to be displayed on the map.
void navcoreFavourites::add(GeocodeResultType type,
                            const TRect *entityBound,
                            const TPos *mapPos,
                            const UINT32 sz)
{
  EntityGeometry tmp;

  tmp.type = type;
  tmp.bBox = *entityBound;
  tmp.pos = *mapPos;
  tmp.size = sz;
  m_favouritesCollection.push_back(tmp);
}

// This marks a favourite for visual highlighting when selected.
void navcoreFavourites::select(const unsigned int pos)
{
  if (m_favouritesCollection.size() > pos) {
    m_currentlySelectedFavourite = pos;
  } else {
    m_currentlySelectedFavourite = -1;
  }
}

// Deselect the currently selected favourite, nothing should be 
// highlighted in this case.
bool navcoreFavourites::deselect(const unsigned int pos)
{
  if (m_currentlySelectedFavourite == pos) {
    m_currentlySelectedFavourite = -1;
    return true;
  } else {
    return false;
  }
}

// Remove all current favourites.
void navcoreFavourites::clearAll(void)
{
  m_currentlySelectedFavourite = -1;
  m_favouritesCollection.clear();
}

void navcoreFavourites::drawBBox(TViewport *viewport, TRect *bBox)
{
  TPos cornerPos;

  cornerPos.x = bBox->xmin;
  cornerPos.y = bBox->ymin;
  Viewport_MoveTo(viewport, &cornerPos);
  cornerPos.x = bBox->xmax;
  cornerPos.y = bBox->ymin;
  Viewport_LineTo(viewport, &cornerPos, RENDERLINE_USEPIXELTHICKNESS);
  cornerPos.x = bBox->xmax;
  cornerPos.y = bBox->ymax;
  Viewport_LineTo(viewport, &cornerPos, RENDERLINE_USEPIXELTHICKNESS);
  cornerPos.x = bBox->xmin;
  cornerPos.y = bBox->ymax;
  Viewport_LineTo(viewport, &cornerPos, RENDERLINE_USEPIXELTHICKNESS);
  cornerPos.x = bBox->xmin;
  cornerPos.y = bBox->ymin;
  Viewport_LineTo(viewport, &cornerPos, RENDERLINE_USEPIXELTHICKNESS);
}

// Render the list of favourites.  Any highlighted favourite will be rendered 
// differently.  Currently only one favourite can be highlighted in a 
// collection.
//
// For each entity draw its bounding box:
// PLACES - bounding box is blue.
// POSTCODES - bounding box is red.
// ROADS - bounding box is white.
void navcoreFavourites::render(TBitmap*myfavouriteicon, TViewport * viewport)
{
  unsigned int i;
  TPos mapPos;
  UINT32 size;
  TDisplayFavourite f;
  TGeometricPen *savedPen;
  TBitmap* bitmap = NULL;

  // Save the current geometric pen before we do anything with our custom
  // bounding box rendering.
  savedPen = Viewport_GetPen(viewport);
  for (i = 0; i < m_favouritesCollection.size(); i++) {
    Viewport_CalcBoundingParameters(viewport, &mapPos, &size, &m_favouritesCollection[i].bBox);
    if (i == m_currentlySelectedFavourite) {
      switch (m_favouritesCollection[i].type) {
        case TYPE_ROAD:
          Favourite_Define(&f, m_selectedIcon, &m_favouritesCollection[i].pos);

          // Render bounding box for ROAD
          Viewport_SetPen(viewport, &m_highlightGeoPen);

          //drawBBox(viewport, &m_favouritesCollection[i].bBox);
          break;

        case TYPE_PLACE:
        case TYPE_POSTCODE:
          Favourite_Define(&f, m_selectedIcon, &m_favouritesCollection[i].pos);

          // Render bounding box for PLACE or POSTCODE
          if (m_favouritesCollection[i].type == TYPE_PLACE) {
            Viewport_SetPen(viewport, &m_highlightGeoPen);
          } else {
            Viewport_SetPen(viewport, &m_highlightGeoPen);
          }

          //drawBBox(viewport, &m_favouritesCollection[i].bBox);
          break;

        case TYPE_OTHER:
          // Entities of types other than place, postcode or road are.
          // not renderable.
          break;
      }
    } else { // This favourite is not selected...
      switch (m_favouritesCollection[i].type) {
        case TYPE_ROAD:
          Favourite_Define(&f, myfavouriteicon, &m_favouritesCollection[i].pos);

          // Render bounding box for ROAD
          Viewport_SetPen(viewport, &m_roadGeoPen);

          //drawBBox(viewport, &m_favouritesCollection[i].bBox);
          break;

        case TYPE_PLACE:
      //    bitmap = GetGlobalPOI(39, 0);
        //  if (bitmap)
            Favourite_Define(&f, myfavouriteicon, &m_favouritesCollection[i].pos);
          break;

		case ICON_TYPE_SEARCH_RESULT:
// 			bitmap = GetGlobalPOI(39, 0);
// 			if (bitmap)
                Favourite_Define(&f, myfavouriteicon, &m_favouritesCollection[i].pos);
			break;

        case TYPE_POSTCODE:
          Favourite_Define(&f, myfavouriteicon, &m_favouritesCollection[i].pos);

          // Render bounding box for PLACE or POSTCODE
          if (m_favouritesCollection[i].type == TYPE_PLACE) {
            Viewport_SetPen(viewport, &m_placeGeoPen);
          } else {
            Viewport_SetPen(viewport, &m_postCodeGeoPen);
          }

          //drawBBox(viewport, &m_favouritesCollection[i].bBox);
          break;

        case TYPE_OTHER:
          // Entities of types other than place, postcode or road are.
          // not renderable.
          break;
      }
    }
    Favourite_Render(&f, viewport);
  }
  // Restore the previously saved geometric pen.
  Viewport_SetPen(viewport, savedPen);
}
