#ifndef _SALCHANNELCONFIGURATION_H_
#define _SALCHANNELCONFIGURATION_H_

#if defined(WIN32) || defined(_WIN32_WCE)
#include <windows.h>
#endif

#define MAX_STRING_PROPERTIES	5
#define MAX_VALUE_PROPERTIES	25


#include "SALTypes.h"

/** @brief Configuration for SALChannel.
  * 
  * Every implementation of a SAL channel has unique configuration. For
  * example, the configuring a serial port is completely different from
  * configuring a TCP socket. This class is not used directly, but is
  * sub-classed by the various different channels to allow setup of the
  * appropriate parameters. The resulting configuration is passed to
  * SALChannelFactory:: to create the actual channel.
  * The subclasses should override setValueProperty(), setStringProperty() etc,
  * as appropriate.
  */


class SALChannelConfig
{
public:
	//! default class constructor
	SALChannelConfig ( void );
	
	//! class destructor
	virtual ~SALChannelConfig ( void );

	//! set an integer property in the configuration. If a subclass does
	//! not override this method, the default method always returns false.
	//! @param id property id to change. See subclass documentation for values
	//! @param value value to set.
	//! @result true if property exists and is set. false otherwise.
	virtual bool setValueProperty ( word_t id, dword_t value) {
		return false;
	}

	//! set a string property in the configuration. Note that most channels
	//! do not copy this string, but only the pointer. If a subclass does
	//! not override this method, the default method always returns false.
	//! @param id property id to change. See subclass documentation for values
	//! @param value value to set.
	//! @result true if property exists and is set. false otherwise.
	virtual bool setStringProperty ( word_t id, const char* string) {
		return false;
	}
#if defined(WIN32) || defined(_WIN32_WCE)
	virtual bool setTextProperty ( word_t id, const TCHAR* string) {
		return false;
	}
#endif
//	
//	//! store the string property str und identifier id
//	bool setStringProperty ( word_t id, char* str );
//	
//	//! return the string str that is stored under the specified id
//	bool getStringProperty ( word_t id, char* str ) const; 
//
//	//! store the value property str und identifier id
//	bool setValueProperty ( word_t id, dword_t value );
//	
//	//! return the string str that is stored under the specified id
//	bool getValueProperty ( word_t id, dword_t* value ) const; 

private:
	//! repository of string properties
//	StringProperty	mStrProperties[MAX_STRING_PROPERTIES];
//	
//	//! number of stored string properties
//	word_t  mNumOfStringProperties;
//	
//	//! repository of value properties
//	ValueProperty  valProperties[MAX_VALUE_PROPERTIES];
//	
//	//! number of stored value properties
//	word_t  mNumOfValueProperties;
};
#endif /*_SALCHANNELCONFIGURATION_H_*/
