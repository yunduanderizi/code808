#pragma once
/** @file 
  \brief 	template class to watch a value and accumulate statistics.

  (c) 2005 Navman New Zealand. all rights reserved.
 
  module name: watch_value.h
 
  author(s): Andrew Watkins

    This template class allows the creation of member variables that monitor
    their contained value and store the min, max, sum and count as the value
    changes. 
    Operators are provided to allow the value to be used in place of an INT32, float or double.

 *****************************************************************************/
#ifndef _WATCH_VALUE_H_
#define _WATCH_VALUE_H_


namespace navman {

/** class to track min max and current value for a measurement
  also keeps track of number of accesses and mean value
 */
template<class _valtype>
	class watch_value
	{	
public:
	typedef watch_value<_valtype> _Myt;

  watch_value() { reset(); }

  /** all operators that change the value call set. this keeps track of the sum, count,min
    and max values.
    @param val new value for internal value.
    */
  void set(_valtype val)
  {
    _access_count++;
    _value = val;
    if (_value > _max) _max = _value;
    if (_value < _min) _min = _value;
    _sum += _value;
  }

	_valtype& operator=(_valtype newval)
		{	// assignment
			set( newval);
  		return (_value);
		}

	_valtype& operator++()
		{	// preincrement
			set( _value + 1);
  		return (_value);
		}

	_valtype& operator--()
		{	// predecrement
			set( _value - 1);
  		return (_value);
		}

	_valtype operator+=(_valtype offset)
		{	
			set( _value + offset );
			return (_value);
		}

	_valtype& operator-=(_valtype offset)
		{	
			set( _value - offset );
  		return (_value);
		}

	_valtype operator++(int)
		{	// postincrement
      _valtype result = _value;
			set(_value + 1);
			return (result);
		}

	_valtype operator--(int)
		{	// postdecrement
      _valtype result = _value;
			set(_value - 1);
			return (result);
		}

  operator _valtype ()
  {
    return (_value);
  }

  const _valtype& minval()    { return _min; }
  const _valtype& maxval()    { return _max; }
  const _valtype& value()  { return _value; }
  INT32 access_count()    { return _access_count; }
  
  _valtype mean()   
  {   
    if (_access_count!=0)
      return _sum / _access_count;

    return (_valtype)0;
  }

  void reset()
  {
    _sum = _min = _max = _value = (_valtype)0;
    _access_count = 0;
  }

private:
  _valtype _min;
  _valtype _max;
  _valtype _value;
  _valtype _sum;
  INT32 _access_count;
};




#ifdef TEST_WATCHVALUE
#include <iostream>

typedef watch_value<INT32> INT32_watch_value;
typedef watch_value<float> float_watch_value;

INT32 _tmain(INT32 argc, _TCHAR* argv[])
{
  INT32_watch_value iwv;
  iwv++;
  ++iwv;
  iwv--;
  --iwv;
  iwv += 10;
  iwv -= 5;
  wcout 
    << L" min=" << iwv.min() 
    << L" max=" << iwv.max() 
    << L" value=" << iwv.value ()
    << L" access=" << iwv.access_count()
    << L" mean=" << iwv.mean()
    << "\n";

  float_watch_value fwv;
  fwv++;
  ++fwv;
  fwv--;
  --fwv;
  fwv += 10.3f;
  fwv -= 5.1f;
  wcout 
    << L" min=" << fwv.min() 
    << L" max=" << fwv.max() 
    << L" value=" << fwv.value ()
    << L" access=" << fwv.access_count()
    << L" mean=" << fwv.mean()
    << "\n";

	return 0;
}
#endif

} // namespace navman

#endif
