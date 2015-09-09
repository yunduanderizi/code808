#pragma once
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__80B05803_3601_433D_A841_A4843E0C7878__INCLUDED_)
#define AFX_STDAFX_H__80B05803_3601_433D_A841_A4843E0C7878__INCLUDED_

#ifndef _POSIX

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _WIN32_WCE   // for windows
//// Modify the following defines if you have to target a platform prior to the ones specified below.
//// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif
#endif  // _WIN32_WCE

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


// TODO: reference additional headers your program requires here
//#include <windows.h>
#include "warnings.h"

#ifndef _WIN32_WCE
//#include <tchar.h>
//#include <MMSystem.h>
#endif

#ifdef _WIN32_WCE
#include <Storemgr.h>
#ifdef TARGET
#ifndef PRODUCT_SDK500
#include <Pkfuncs.h>
#include <ras.h>
#endif // !PRODUCT_SDK500
#include <Tlhelp32.h>
#endif // TARGET
#include <Notify.h>
#include <Pm.h>
#endif // _WIN32_WCE

#ifndef MAX_TEXT
#define MAX_TEXT 256
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !_POSIX

#endif // !defined(AFX_STDAFX_H__80B05803_3601_433D_A841_A4843E0C7878__INCLUDED_)

