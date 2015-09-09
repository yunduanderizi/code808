#pragma once
#ifndef _XMLFILE_H_
#define _XMLFILE_H_

#include "object.h"

namespace navman {

class XmlFilePrivate;
class XmlChildListPrivate;
class XmlChildList;
class XmlMemFileImpl;

/**
Interface for a node object of an XML file.
MSXML classifies all the different types of information in the 
XML file as nodes. 
*/
class IXmlNode : public IObject
{
  friend class XmlChildList;
  friend class XmlChildListPrivate;

public:
  /**
  gets the name of this node
  @param[out] name buffer to receive the name
  @param pLen pointer to the length of the buffer
  @return true on success, false otherwise
  */
  virtual bool getNodeName( WCHAR *name, UINT32 *pLen ) = 0;

  /**
  gets the attribute value for this node
  @param name name of the attribute to retrieve
  @param[out] valBuf buffer to receive value of the attribute
  @param pLen pointer to the length of the buffer
  @return true on success, false otherwise
  */
  virtual bool getAttribute( const WCHAR *name, WCHAR *valBuf, UINT32 *pLen ) = 0;

  /**
  gets the next attribute for this node - like an iterator
  returns false if there are no more attributes for that node
  @param[out] nameBuf buffer to receive the name of the attribute
  @param nameLen pointer to the length of the buffer to hold the name
  @param[out] valBuf buffer to receive the value of the attribute
  @param valLen pointer to the length of the buffer to hold the value
  @param first true if the first attribute is to be retrieved, false otherwise
  @return true on success, false otherwise
  */
  virtual bool getNextAttribute( WCHAR *nameBuf, UINT32 *pNameLen, WCHAR *valBuf, UINT32 *pValLen, bool first ) = 0;

  /**
  gets the text for this node
  @param[out] buf buffer to hold the text for this node
  @param pLen pointer to the length of the buffer
  @return true on success, false otherwise
  */
  virtual bool getText( WCHAR *buf, UINT32 *pLen ) = 0;

  /**
  gets the number of child nodes for this node
  @param[out] pNum number of children for this node
  @return true on success, false otherwise
  */
  virtual bool getNumChildren( UINT32 *pNum ) = 0;

  /**
  gets the name of a node given a path to that node
  @param path buffer holding an XPATH or url to the node required
  @param[out] nameBuf buffer to hold the name of the node
  @param pLen pointer to the length of the buffer to hold the name
  @return true on success, false otherwise
  */
  virtual bool getNode( const WCHAR *path, WCHAR*nameBuf, UINT32 *pLen ) = 0;

  /**
  sets the text for this node - fails if this node has any children or those will get replaced too
  @param text buffer holding the text to be set
  @return true on success, false otherwise
  */
  virtual bool setText( const WCHAR *text) = 0;

  /**
  adds an attribute to this node or changes the value for the specified attribute
  @param name name of the attribute to be added or modified
  @param value value of the attribute to be added or changed
  @return true on success, false otherwise
  */
  virtual bool setAttribute( const WCHAR *name, const WCHAR *value ) = 0;

#if 0
  /**
  adds a comment to this node
  @param comment buffer holding the comment to be added
  @return true on success, false otherwise
  */
  virtual bool setComment( const WCHAR *comment ) = 0;
#endif	//0

  /**
  removes an attribute (name and value) from this node
  @param name name of the attribute to be removed
  @return true on success, false otherwise
  */
  virtual bool removeAttribute( const WCHAR *name ) = 0;

  /**
  removes the text for this node - fails if this node has any children or those will be removed also
  @return true on success, false otherwise
  */
  virtual bool removeText() = 0;

protected:
  //deliberately undocumented - internal function
  virtual bool getChildList( void **ppList ) = 0;

  //deliberately undocumented - internal function
  virtual bool getNamedChildList( const WCHAR *tagName, void **ppList ) = 0;
  
  //deliberately undocumented - internal function
  virtual bool getXmlDomDoc( void **ppXmlDoc ) = 0;

  //deliberately undocumented - internal function
  virtual bool getXmlDomNode( void **ppXmlDomNode ) = 0;
  
  //deliberately undocumented - internal function
  virtual bool addChildNode( const WCHAR *name, void **ppNew ) = 0;

  //deliberately undocumented - internal function
  virtual bool removeNode() = 0;
};

/**
XmlNode object
*/
class XmlNode : public IXmlNode
{
  friend class XmlFilePrivate;
  friend class XmlChildListPrivate;
  friend class XmlMemFileImpl;

private:
  /**
  Factory function to return the XmlNode object - called from within XmlFile
  @param pDomDoc pointer to the DOM document that is to be bound to the new object
  @param[out] ppObj pointer to receive the new object
  @return true on success, false otherwise
  */
  static bool create( void *pDomDoc, XmlNode **ppObj );

  /**
  Factory function to return the XmlNode object
  returned as an IXmlNode interface.
  see XmlNode::create( void *pDomDoc, XmlNode **ppObj );
  */
  static bool create( void *pDomDoc, IXmlNode **ppObj );

  /**
  Factory function to return the XmlNode object - called from within XmlChildList
  Only used when generating a child list - to bind the DOM node to the XmlNode object
  @param pDomDoc pointer to the DOM document to be bound to the new object
  @param pDomNode pointer to the DOM node to be bound to the new object
  @param[out] ppObj pointer to receive the new object
  @return true on success, false otherwise
  */
  static bool create( void *pDomDoc, void *pDomNode, XmlNode **ppObj );

  /**
  Factory function to return the XmlNode object returned as an IXmlNode interface
  - called from within XmlChildList
  see XmlNode::create( void *pDomDoc, void *pDomNode, IXmlNode **ppObj );
  */
  static bool create( void *pDomDoc, void *pDomNode, IXmlNode **ppObj );
};

/**
Interface for an XML file object - handles loading, parsing and closing the XML file.
Also handles retrieving information about the nodes
*/
class IXmlFile : public IObject
{
public:
  /**
  closes and saves the XML file in memory
  @return true on success, false otherwise
  */
  virtual bool closeXmlFile() = 0;

  /**
  gets the XmlNode object for a node given the url to it
  @param path url to the node required
  @param[out] nameBuf buffer to hold the name of the node retrieved
  @param pLen pointer to the length of the name buffer
  @param[out] ppNode pointer to the new XmlNode object
  @return true on success, false otherwise
  */
  virtual bool getNode( const WCHAR *path, WCHAR *nameBuf, UINT32 *pLen, XmlNode **ppNode ) = 0;

  /**
  gets the XmlNode object for a node given the url to it
  @param path url to the node required
  @param[out] nameBuf buffer to hold the name of the node retrieved
  @param pLen pointer to the length of the name buffer
  @param[out] ppNode pointer to the new XmlNode object
  @return true on success, false otherwise
  */
  virtual bool getNode( const WCHAR *path, WCHAR *nameBuf, UINT32 *pLen, IXmlNode **ppNode ) = 0;

  /**
  accessor for the name of the XML file that has been loaded into this XmlFile object
  @param[out] buf buffer holding the name of the file loaded
  @param pLen pointer to the length of the file name buffer
  @return true on success, false otherwise
  */
  virtual bool getFileName( WCHAR *buf, UINT32 *pLen ) = 0;

  /**
  gets whether changes will be persisted to the file when it is closed (i.e. if read only access).
  @return true if changes will not be persisted (read only), false otherwise.
  */
  virtual bool getDontPersist() const = 0;

#if 0
  /**
  sets the comment for an XML file
  @param buf buffer holding the comment to be added
  @return true on success, false otherwise
  */
  virtual bool setComment( const WCHAR *comment ) = 0;
#endif	//0

};


/**
XmlFile object
*/
class XmlFile : public IXmlFile
{
public:
  /**
  Factory function to return the XmlFile object
  @param name buffer holding the name of the XML file to open
  @param[out] ppObj pointer to receive the new object
  @return true on success, false otherwise
  */
  static bool create( const WCHAR *name, XmlFile **ppObj );

  /**
  Factory function to return the XmlFile object
  returned as an IXmlFile interface.
  see XmlFile::create( const WCHAR *name, XmlFile **ppObj );
  */
  static bool create( const WCHAR *name, IXmlFile **ppObj );

  /**
  Factory function to return the XmlFile object
  @param name buffer holding the name of the XML file to open
  @param dontPersist true if the XML file is to be opened for read access only (file will not be saved when closing),
      false if write access also required.
      If true, changes will still occur in memory but will not be persisted when the file is closed.
  @param[out] ppObj pointer to receive the new object
  @return true on success, false otherwise
  */
  static bool create( const WCHAR *name, bool dontPersist, XmlFile **ppObj );

  /**
  Factory function to return the XmlFile object
  returned as an IXmlFile interface.
  see XmlFile::create( const WCHAR *name, bool dontPersist, XmlFile **ppObj );
  */
  static bool create( const WCHAR *name, bool dontPersist, IXmlFile **ppObj );

  /***************************************************************************/  
  /** \brief creates an empty xml file
  *
  * this will create an empty xml file that is of minimal set. the reason is that
  * the serializer must work with file that already exist. However, this function
  * is only used when the user explicitly wants to create an empty file only.
  * This allows the user to do something without writing any further objects to it
  *
  *  @param wcXmlFilePath the path to the file to be create
  *  @param wcobjectName the name of the root node in the file
  ***************************************************************************/
  static bool writeEmptyFile(const WCHAR *wcXmlFilePath, const WCHAR *wcObjectName);
};


/**
Interface for a list of child nodes of an XmlNode
This list is created when a child node of a node is to be accessed. 
It provides the child nodes as XmlNode objects that can be manipulated independently
*/
class IXmlChildList : public IObject
{
public:
  /**
  gets the length of the list of child nodes
  @param[out] pLen the length of the list
  @return true on success, false otherwise
  */
  virtual bool getLength( UINT32 *pLen )= 0;

  /**
  gets the length of the list of child nodes
  @param[out] pLen the length of the list
  @return true on success, false otherwise
  */
  virtual UINT32 getLength() const = 0;

  /**
  gets the next child in the list - can be used like an iterator
  Returns false if there are no more nodes in the child list.
  @param bFirst boolean indicating if the first child node is to be retrieved
  	if true - get the first child node
  	if false - get the node that the index is currently pointing to
  @param[out] ppChildNode pointer to the child XmlNode object
  @return true on success, false otherwise
  */
  virtual bool getNextChild( bool bFirst, XmlNode **ppChildNode ) = 0;

  /**
  gets the next child in the list - can be used like an iterator
  Returns false if there are no more nodes in the child list.
  @param bFirst boolean indicating if the first child node is to be retrieved
  	if true - get the first child node
  	if false - get the node that the index is currently pointing to
  @param[out] ppChildNode pointer to the child XmlNode object
  @return true on success, false otherwise
  */
  virtual bool getNextChild( bool bFirst, IXmlNode **ppChildNode ) = 0;

  /**
  adds a node to the existing list of child nodes
  @param pParent pointer to the parent XmlNode object that the child list belongs to
  @param name buffer holding the name of the new child node to be added
  @param[out] ppNewNode pointer to the new XmlNode object that has been created and added
  @return true on success, false otherwise
  */
  virtual bool addNode( IXmlNode *pParent, const WCHAR *name, XmlNode **ppNewNode ) = 0;

  /**
  removes a node from this list of child nodes. Also updates the number of child nodes for the parent XmlNode object
  @param pParentNode pointer to the parent XmlNode object
  @param ppChildNode pointer to the child XmlNode object to be removed
  @return true on success, false otherwise
  */
  virtual bool removeChild( IXmlNode *pParentNode, XmlNode *pChildNode ) = 0;
};


/**
XmlChildList object
*/
class XmlChildList : public IXmlChildList
{
public:
  /**
  Factory function to return the XmlChildList object
  @param pParentNode pointer to the parent XmlNode object that the list of children belong to
  @param[out] ppObj pointer to the new XmlChildList object created
  @return true on success, false otherwise
  */
  static bool create( IXmlNode *pParentNode, XmlChildList **ppObj );

  /**
  Factory function to return the XmlChildList object returned as an IXmlChildList interface object
  see XmlChildList::create( IXmlNode *pParentNode, XmlChildList **ppObj );
  */
  static bool create( IXmlNode *pParentNode, IXmlChildList **ppObj );

  /**
  Factory function to return the XmlChildList object.
  This is called when a list is required of child nodes with a specific name, and not all child nodes of that node.
  Eg: when creating a property bag - to differentiate between properties and child property bags
  @param pParentNode pointer to the parent XmlNode object
  @param tagName buffer holding the name of the child nodes required
  @param[out] ppObj pointer to the created XmlChildList object
  @return true on success, false otherwise
  */
  static bool create( IXmlNode *pParentNode, const WCHAR *tagName, XmlChildList **ppObj );

  /**
  Factory function to return the XmlChildList object returned as an IXmlChildList interface object
  see XmlChildList::create( IXmlNode *pParentNode, const WCHAR *tagName, XmlChildList **ppObj );
  */
  static bool create( IXmlNode *pParentNode, const WCHAR *tagName, IXmlChildList **ppObj );

  /**
  Factory function to return the XmlChildList object.
  This function is to be called if the parent node does not have any existing child nodes.
  This function adds a node at the same time as creating a list with that node in it.
  @param pParentNode pointer to the parent XmlNode object that the list is created for
  @param name buffer holding the name of the new child node to be added
  @param[out] ppNewNode pointer to the new XmlNode object created and added to the list
  @param[out] ppObj pointer to the new XmlChildList object created
  @return true on success, false otherwise
  */
  static bool create( IXmlNode *pParentNode, const WCHAR *name, XmlNode **ppNewNode, XmlChildList **ppObj );

  /**
  Factory function to return the XmlChildList object as an IXmlChildList interface
  see XmlChildList::create( IXmlNode *pParentNode, const WCHAR *name, XmlNode **ppNewNode, XmlChildList **ppObj );
  */
  static bool create( IXmlNode *pParentNode, const WCHAR *name, XmlNode **ppNewNode, IXmlChildList **ppObj );

};


} // namespace navman

#endif // _XMLFILE_H_
