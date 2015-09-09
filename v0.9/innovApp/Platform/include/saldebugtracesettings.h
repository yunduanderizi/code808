#ifndef _SALDEBUGTRACESETTINGS_H_
#define _SALDEBUGTRACESETTINGS_H_

/** @file SALDebugTraceSettings.h
  * @brief Define Debug Groups
  */

enum enDebugGroup 
{
	DT_GLOBAL = 0,
	DT_SAL,					/**< system abstraction layer related */
	DT_PM,					/**< process manager related */
	DT_GPS,					/**< sensor manager related */
	DT_NAV,					/**< navigation engine application related */
	DT_TMC,					/**< traffic message channel related */
	DT_NAV_VP,				/**< positioning module in navigation engine application */
	DT_NAV_DI,				/**< display module in navigation engine application */
	DT_NAV_TR,				/**< traffic module in navigation engine application */
	DT_NAV_GC,				/**< geocoding module in navigation engine application */
	DT_NAV_RC,				/**< route calculation module in navigation engine application */
	DT_NAV_GD,				/**< guidance module in navigation engine application */
	DT_ECM,					/**< external communication manager related */
	DT_UNITTEST,			/**< unit test related */
	DT_NUMBER_OF_GROUPS		/**< maximum is 256 */
};
 
#endif /*_SALDEBUGTRACESETTINGS_H_*/
