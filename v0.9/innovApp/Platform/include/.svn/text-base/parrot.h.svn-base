#pragma once
/** \file parrot.h
  * This file contains the API interface to control the parrot module. 
  */
#ifndef PARROT_H
#define PARROT_H

/*	Defines And Structures
 *	------------------------------
 */
#define MAX_ACTIVE_CALLS          2 /* Maximum number of supported simultaneous calls */
#define PARROT_ACTIVE_CALL_ONE    0
#define PARROT_ACTIVE_CALL_TWO    1


/**	General Menu Return Codes
 */
typedef enum {
  PARROT_OPER_OK = 0,       /**< The Parrot Module operation was successful */
  PARROT_OPER_FAIL,         /**< The Parrot Module operation did not succeed */
  PARROT_OPER_PARM_INVALID, /**< A parameter passed was not valid*/
  PARROT_OPER_NOT_PROCESSED /**< Valid inputs were presented, but no processing was required */	
} parrotErrCodeT;

/* Pop-up Icon Enums */
 typedef enum {
  PARROT_MODULE_ACTIVE_CALL = 0,
  PARROT_MODULE_HELD_CALL,
  PARROT_MODULE_OUTGOING_CALL_DIALING,
  PARROT_MODULE_OUTGOING_CALL_RINGING,
  PARROT_MODULE_INCOMING_RINGING,
  PARROT_MODULE_WAITING_CALL,
  PARROT_MODULE_CALL_TERMINATED,
  PARROT_MODULE_NO_STATUS
} callStateT;

typedef enum {
  PARROT_MODULE_GENERAL_PHONE = 0,
  PARROT_MODULE_HOME_PHONE,
  PARROT_MODULE_OFFICE_PHONE,
  PARROT_MODULE_MOBILE_PHONE,
  PARROT_MODULE_OTHER_PHONE,
  PARROT_MODULE_NO_TYPES,
} callPhoneTypeT;

/**	Caller Id structure contains the ID name / number string and call status data.
 */
typedef struct {
  char CallerID[50];        /**< String displaying caller name or phone number */
  callPhoneTypeT PhoneType; /**< Address book entry type */
  callStateT CallStatus;    /**< Status of call e.g. Incoming, outgoing... */
} parrotCallerIDTypeT;

 
typedef struct {
  UINT8   numberOfCalls;
  UINT32  popupTimeout;
  parrotCallerIDTypeT parrotCallerIDType[MAX_ACTIVE_CALLS];
} parrotPopupT;

#ifdef HAS_FEATURE_ONE2ONE

/**	Parrot Public Routines
 */
parrotErrCodeT parrotProcessRx(char *p_strData, int p_nLength);
parrotErrCodeT parrotProcessRxByte(char byte);
parrotErrCodeT parrotInit(void);

#endif // HAS_FEATURE_ONE2ONE

#endif /* PARROT_H */
