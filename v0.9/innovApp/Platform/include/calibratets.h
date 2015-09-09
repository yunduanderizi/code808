#pragma once
/*****************************************************************************
 *  (c) 2005 navman new zealand limited. all rights reserved.
 *
 *  module name: CalibrateTS.h
 *
 *  author(s): Tom Isaacson <tisaacson@navman.com>
 *
 *  description:
 *    This module initialises the touchscreen
 *****************************************************************************/

#ifndef _CALIBRATETS_H_
#define _CALIBRATETS_H_


#ifdef _WIN32_WCE
BOOL wince_calibrateTouchScreen(BOOL p_bUserSet);
#endif // _WIN32_WCE


#endif // _CALIBRATETS_H_
