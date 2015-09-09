#pragma once
/**
  @file
  Contains XML Memory File interface declaration.
  Copyright (C) 2006, Navman New Zealand.  All rights Reserved.
*/
#ifndef _XMLMEMFILE_H_
#define _XMLMEMFILE_H_

#include "object.h"     // for IObject
#include "xmlfile.h"    // for IXmlNode etc

namespace navman 
{
  /**
    Interface for an XML Memory Object - handles loading and parsing of the XML text.
    Also handles retrieving information about the nodes.
    This component is VERY similar to IXmlFile except that it loads from a text
    string instead of a file. Also, all of the unnecessary file handling has been removed.
  */
  class IXmlMemFile : public IObject
  {
  public:

    /**
      Gets the XmlNode object for a node given the url to it
      @param path url to the node required
      @param[out] nameBuf buffer to hold the name of the node retrieved
      @param pLen pointer to the length of the name buffer
      @param[out] ppNode pointer to receive the new XmlNode object
      @return true on success, false otherwise
    */
    virtual bool getNode( const WCHAR *path, WCHAR *nameBuf, UINT32 *pLen, XmlNode **ppNode ) = 0;

    /**
      Gets the XmlNode object for a node given the url to it
      @param path url to the node required
      @param[out] nameBuf buffer to hold the name of the node retrieved
      @param pLen pointer to the length of the name buffer
      @param[out] ppNode pointer to receive an interface for the new XmlNode object
      @return true on success, false otherwise
    */
    virtual bool getNode( const WCHAR *path, WCHAR *nameBuf, UINT32 *pLen, IXmlNode **ppNode ) = 0;
  };

  /**
    Creator class exposing the IXMLMemFile interface.
  */
  class XmlMemFile
  {
  public:
    /**
      Factory function to create and return the XmlFile object as an IXmlMemFile.
      @param pXMLString String that represents the XML source.
      @param[out] ppObj pointer to receive the new object
      @return true on success, false otherwise
    */
    static bool create( const WCHAR * pXMLString, IXmlMemFile **ppObj );
  };

} // namespace navman 

#endif  // _XMLMEMFILE_H_

