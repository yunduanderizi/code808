#pragma once

#ifndef __KEYHOOK_H__
#define __KEYHOOK_H__

#if defined(_WIN32_WCE) && !defined(PRODUCT_SDK500)

#include <pwinuser.h>

#define LLKHF_EXTENDED  0x01
#define LLKHF_INJECTED  0x10
#define LLKHF_ALTDOWN   0x20
#define LLKHF_UP        0x80

typedef bool TKeyHookFilter(int p_nCode, WPARAM p_wParam, KBDLLHOOKSTRUCT *p_pKbDllData);

bool wince_installHook(TKeyHookFilter *p_pFilterFunc);
void wince_uninstallHook(void);

#endif // _WIN32_WCE && !PRODUCT_SDK500

#endif // __KEYHOOK_H__
