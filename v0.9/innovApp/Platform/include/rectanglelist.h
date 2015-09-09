#pragma once
/**
  @file
  Contains the definition of IRectangleList.
  Copyright (C) 2005.  Navman NZ Ltd.  All rights reserved.
*/

#ifndef _RECTANGLELIST_H_
#define _RECTANGLELIST_H_

#include "object.h"
#include "math/gerect.h"
#include "persist.h"
#include "mapcore_types.h"

namespace navman {

class ISerialize;

/**
  This interface provides access to a list of rectangles.
*/
class IRectangleList : public IObject
{
public:
  /**
    Gets the rectangle at the specified index.  Note: the results are
    undefined in the index is outside the list.
    @param index the item to retrieve.
    @return the rectangle
  */
  virtual const TRect& get(UINT32 index) const = 0;

  /**
    Gets the first bounding rectangle found in the list.
    @param pos the point (in latitude/longitude) the returned rectangle should bound.
    @param searchFromEnd true if the search should start from the back of the list,
                         false if from the beginning.
    @return pointer to the first bounding rectangle found, NULL if no bounding rectangle is found.
  */
  virtual const TRect *getBounding(TPos& pos, bool searchFromEnd) const = 0;

  /**
    Gets the count of rectangles in the list
    @return count of items.
  */
  virtual UINT32 getCount() const = 0;

  /**
    Adds a rectangle to the end of the list.
    @param rect the rectangle to add.
    @return true on success, false otherwise.
  */
  virtual bool add(const TRect& rect) = 0;

  /**
    Removes the first matching rectangle from the list.  This is accomplished
    by doing a comparison on the four corners of the rectangle.
    @param rect the rectnagle to remove.
    @return true on success removal, false on failure or if the item doesn't exist.
  */
  virtual bool remove(const TRect& rect) = 0;
};

/**
  Component exporting the IRectangleList interface and containing the
  creator functions.
*/
class RectangleList : public IRectangleList, public IPersist
{
public:
  /**
    Factory function to create IRectangeList objects.
    @param[out] ppObj pointer to receive the new object.
    @return true on success, false otherwise.
  */
  static bool create(IRectangleList **ppObj);

  /** Deserialization creator for a Persisted Rectangleist Object
     @param pSerializer the serializer with the data
     @param [out] ppPersistable define where to output the deserialized data
     @return true on success, else false
  */
  static bool create(ISerialize *pSerializer, IPersist **ppPersistable);  
};

} // namespace navman

#endif // _RECTANGLELIST_H_

