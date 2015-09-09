#ifndef AURIMAGEP_H
#define AURIMAGEP_H

#include <authentication/aurimage.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ACTPKCS11_
void process_ret_code( CK_RV rc );
void show_error( CK_BYTE *str, CK_RV rc, int iThreadID, int iSessionSub );
int initCryptoCard();
int checkInitCryptoCard();
#endif

#ifdef __cplusplus
}
#endif

#ifdef ACTIVATION_SERVER_
#include <stdio.h>
#define os_printf printf
#endif

#endif /* AURIMAGEP_H */
