#ifndef INCLUDE_UTF8_H_
#define INCLUDE_UTF8_H_

#include "types.h"

typedef struct {
	byte data[4];
} Octet;

uint  octet_len(byte b);
Octet encode_rune(rune r);
rune  decode_octet(Octet oc);
bool  octet_validate(Octet oc);

#endif /* include guard */
