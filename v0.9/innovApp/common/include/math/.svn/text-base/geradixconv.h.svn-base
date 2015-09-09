#ifndef _geradixconv_h
#define _geradixconv_h

#define MAX_RADIXCONV_DIGITS 1000

#ifdef __cplusplus
extern "C" {
#endif

#include <platform.h>

// arbitrary-radix numbers are represented least-significant-digit first in the 'digits' array - the base 10
// number 1234 would be represented as byte [0] = 0x04, byte [1] = 0x03, byte [2] = 0x02 and byte [3] 0x01.
struct TRadixNumber {

    UINT32 digitCount;
    UINT8 digits[MAX_RADIXCONV_DIGITS];
};

// radixnum_zero(): initialise a radix number.
void radixnum_zero(struct TRadixNumber *a);

// radixnum_convert(): dest = source converted from source_radix to dest_radix
void radixnum_convert(struct TRadixNumber *dest, UINT8 dest_radix, struct TRadixNumber *source, UINT8 source_radix);

// convert16to36(): Sample use of radixnum_convert that converts a null-terminated ASCII string representing a number in base 16
// ('0' - '9', 'A' - 'F', 'a' - 'f') to a string representing the same number in base 36 ('0' - '9', 'A' - 'Z').
void convert16to36(const char *base16String, char *base36Buffer, UINT32 bufferSize);

#ifdef __cplusplus
}
#endif

#ifdef ACTIVATION_SERVER_
#include <stdio.h>
#define os_printf printf
#endif

#endif
