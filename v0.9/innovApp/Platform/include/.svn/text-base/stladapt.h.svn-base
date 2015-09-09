#pragma once
/**
  @file

  Class to Adapt CComBSTR and CComPtr for use with STL containers
  the syntax to use it is
  std::vector< CAdapter <CComBSTR> > vect;

  [This was taken directly from the ATL template CAdapt in atlcomcli.h]

  Copyright (C) 2007, Navman New Zealand.  All rights Reserved.
*/

#ifndef _STLADAPT_H_
#define _STLADAPT_H_

namespace navman
{

template <class T>
class CAdapter
{
public:

  /**
    Construct an adapter.
  */
  CAdapter()
  {
  }
  
	/**
    Construct an adapter around an existing class.
    @param rSrc Existing class to adapt.
  */
  CAdapter(__in const T& rSrc) :
    m_T( rSrc )
  {
  }

  /**
    Copy construction.
    @param rSrCA Adapter to copy.
  */
  CAdapter(__in const CAdapter& rSrCA) :
    m_T( rSrCA.m_T )
  {
  }

  /**
    Map the assignment operator to that of the underlying template parameter.
    @param rSrc The templated class being adapted.
    @return Reference to the l.h.s.
  */
  CAdapter& operator=(__in const T& rSrc)
  {
    m_T = rSrc;
    return *this;
  }

  /**
    Map the comparison operator to that for the underlying template parameter.
    @param rSrc R.h.s of the comparison.
    @return The result for comparison of the underlying template parameter.
  */
  bool operator<(__in const T& rSrc) const
  {
    return m_T < rSrc;
  }

  /**
    Map the equality operator to that of the underlying templated class.
    @param rSrc R.h.s for equality test.
    @return The result of the equality test for the underlying template parameter.
  */
  bool operator==(__in const T& rSrc) const
  {
    return m_T == rSrc;
  }

  /** 
    Conversion operator.
    @return Reference to the underlying template parameter.
  */
  operator T&()
  {
    return m_T;
  }

  /** 
    Conversion operator.
    @return Reference to the underlying template parameter.
  */
  operator const T&() const
  {
    return m_T;
  }

  T m_T;
};

} // namespace navman

#endif  // _STLADAPT_H_
