#ifndef _SALCHANNELCONFIGWIN_H_
#define _SALCHANNELCONFIGWIN_H_

#include "libSAL.h"
#include "SALChannelConfig.h"

/** @brief SAL Channel configuration for Windows CE MsgQueue
  */

class SALChannelConfigWin
	: public SALChannelConfig
{
	friend class SALChannelImpWin;
public:
	/** Create a SALChannelConfigWin. This is passed to SALChannelFactory::registerImp()
	  */
	static SALChannelConfig* config();

	/** IDs for setTextProperty().
	  */
	typedef enum {
		NAME=1		/**< Name of channel. Defaults to "SALChannel".
					 * The AppID is appended to the end of this string when opened.
					 */
	} configType;

	/** Set properties of the configuration.
	  * @param config a value from the enum configType
	  * @param value the string to set the parameter to.
	  * @return true if the property was recognised and set
	  */
	bool setTextProperty(word_t config, const TCHAR* value);

private:
	//! constructor 
	SALChannelConfigWin ( );
	
	//! class destructor
	~SALChannelConfigWin ( void );

private:
	const TCHAR* mQueueName;
};




#endif
