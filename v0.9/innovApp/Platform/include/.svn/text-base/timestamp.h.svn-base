#pragma once
/** @file 
  The TimeStamp class stores a time and date value that doesn't change.

  The time and date is set when the object is created.  The object can them be associated with other
  data to determine when that data was set or updated.
      
   This file declares:
  
      - ITimeStamp : Interface used to store a time and date

      - TimeStamp  : Creator class for objects with the ITimeStamp interface.  

  (c) 2005 navman new zealand limited. all rights reserved.
*/
    

#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_

#include "object.h"

namespace navman
{


/**
    Interface to store a particular time and date.

  TimeStamp stores a time and date, and can be used to record the time when events are sent
  or data updated.
*/
class ITimeStamp: public IObject 
{
public:

  /**
     Get the date recorded in the object.
     @return Date represented in a 32 bit package
  */
  virtual UINT32 getDate() const = 0;


  /**
     Get the time recorded in the object as the number of seconds elapsed 
     since the start of the epoch (1st January 1970).
     @return Seconds since Jan 1st 1970.
  */
  virtual UINT32 getTimeSinceEpoch() const = 0;


  /**
     Get the UTC time recorded in the object in milliseconds since midnight.
     @return Milliseconds since midnight
  */
  virtual UINT32 getTimeSinceMidnight() const = 0;

  /**
     @deprecated
     Use getTimeSinceMidnight()
     Get the UTC time recorded in the object in milliseconds since midnight.
     @return Milliseconds since midnight
  */
  virtual UINT32 getTime() const = 0;

  /**
     create a deep copy of this object
     @param[out] ppObj pointer to receive the copied object
     @return true on success, false otherwise
  */
  virtual bool clone(ITimeStamp **ppObj) const = 0;
};


/**
    TimeStamp factory class.  
    See ISelectedLocation for more details.
*/
class TimeStamp: public ITimeStamp
{
public:

  /**
     Factory function to create a TimeStamp object
     @param [out] ppObj Pointer to receive the new object.
     @param [in] timeSinceEpoch Time, in seconds since 1st Jan 1970.
     @param [in] timeSinceMidnight Time, in milliseconds since midnight.
     @param [in] utcTime Time, in milliseconds since midnight.
     @param [in] date Date, as a 32 bit package.
     @return true on success, false otherwise
  */
  static bool create( UINT32 timeSinceEpoch, UINT32 timeSinceMidnight, UINT32 date, TimeStamp **ppObj );

  /**
     Factory function to create a TimeStamp object, returned 
     as an ITimeStamp interface
     See create( UINT32 timeSinceEpoch, UINT32 timeSinceMidnight, UINT32 date, TimeStamp **ppObj )
  */
  static bool create(UINT32 timeSinceEpoch, UINT32 timeSinceMidnight, UINT32 date, ITimeStamp **ppObj);
};




} // namespace navman

#endif // _TIMESTAMP_H_

