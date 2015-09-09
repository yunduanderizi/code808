#pragma once
#ifndef _MONIKER_H_
#define _MONIKER_H_

#include "object.h"
#include "serialize.h"
#include "persist.h"

#define MONIKER_MAX_NAME 1024

namespace navman
{
  /**
     IMoniker interface. A moniker is used to name an object.
     A moniker points to an object and holds a name. The IMoniker
     interface is common to all monikers. 
  */
  class IMoniker : public IObject
  {
  public:
    /**
      retrieves the name of the current moniker.
      @param wszName pointer to buffer to copy moniker name into
      @param len length of buffer
      @return true on success, false otherwise
    */
    virtual bool getName(WCHAR *wszName, UINT32 len) = 0;

    // Here, the IMoniker interface could have a bool getObject(IObject **ppObj) method
    // however a type-safe object reference to the object the moniker binds to
    // is better declared in moniker classes, such as XmlNodeMoniker, below.
  };

  /**
     IXmlNodeMoniker interface. This interface declares methods
     relative to monikers holding references to XML nodes.
  */
  class IXmlNodeMoniker : public IObject // TODO: IXmlNodeMoniker could well derive from IMoniker, this is not an absolute necessity.
  {
  public:
    /**
      retrieves the XML node which this moniker binds to.
      @param[out] ppObj holds a reference to the node bound to the moniker on return
      @return true on success, false otherwise
   */
    virtual bool getXmlNode(XmlNode **ppNode) = 0;
  };

  /**
     Moniker class interface. This class only has a method serving
     as a gateway to create instances of various moniker classes.
  */
  class Moniker : public IMoniker, public IXmlNodeMoniker
  {
  public:
    /**
      Factory function to create a XML node moniker.
      @param name the name of the moniker,
      @param pXmlNode the node to which this moniker is bound,
      @param[out] ppObj holds a reference to the newly created moniker on return
      @return true on success, false otherwise
    */
    static bool create(const WCHAR *name, XmlNode *pXmlNode, IMoniker **ppObj);
  };
}

#endif // _MONIKER_H_

