#ifndef AUSIGNP_H
#define AUSIGNP_H

#include <authentication/ausign.h>

#ifdef __cplusplus
extern "C" {
#endif

#define E_JNI_EED_ENCRYPT_B4_ADD     -70222
#define E_JNI_EED_NO_DATA_TO_ENCRYPT -70223
#define E_JNI_EED_KEY_ALREADY_ADDED  -70224
#define E_JNI_EED_ADD_B4_INIT_       -70229

#define PK_LEGACY   0
#define PK_TUI      1
#define PK_WDD      2
#define PKCOUNT     3
 
#define CRYPTOPROVIDER_OPENSSL_         1
#ifdef ACTPKCS11_
#define CRYPTOPROVIDER_PKCS11_          2
#endif
#define CRYPTOPROVIDER_NAVMAN_REF_      3

//#ifdef ACTPRINTBUFFERS_
//#ifdef ACTDEBUG_
#define ACTPRINTBUF_( HDR_, BUF_, LEN_ )       \
    {                                          \
        int i = 0;                             \
                                               \
        os_printf(HDR_);                       \
        for( i = 0; i < (LEN_); i++ ) {        \
            os_printf(" %2.2X", (BUF_)[i] );   \
        }                                      \
        os_printf("\n");                       \
    }
//#else
//#define ACTPRINTBUF_( HDR_, BUF_, LEN_ )
//#endif
//#else
//#define ACTPRINTBUF_( HDR_, BUF_, LEN_ )
//#endif

#ifdef __cplusplus
}
#endif

#ifdef ACTIVATION_SERVER_
#include <stdio.h>
#define os_printf printf
#endif

#endif /* AUSIGNP_H */
