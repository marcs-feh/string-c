#include "utf8.h"

static const u32 utf8_range_1byte = 0x0000007f;
static const u32 utf8_range_2byte = 0x000007ff;
static const u32 utf8_range_3byte = 0x0000ffff;
static const u32 utf8_range_4byte = 0x0010ffff;

// Get length of an octet based on its first byte
uint octet_len(byte b){
	if (!(b & 0x80)){ return 1; }
	else if((b >> 5) == 0x06) { return 2; }
	else if((b >> 4) == 0x0e) { return 3; }
	else if((b >> 3) == 0x1e) { return 4; }
	else { return 0; }
}

// Encode rune to octet
Octet encode_rune(rune r){
	Octet oc = {0};
	if(r <= utf8_range_1byte){
		// 0xxxxxxx
		oc.data[0] = r;
	}
	else if(r <= utf8_range_2byte){
		// 110xxxxx 10xxxxxx
		oc.data[1] = (r & 0x3f) | 0x80;
		r = r >> 6;
		oc.data[0] = r | 0xc0;
	}
	else if(r <= utf8_range_3byte){
		// 1110xxxx 10xxxxxx 10xxxxxx
		oc.data[2] = (r & 0x3f) | 0x80;
		r = r >> 6;
		oc.data[1] = (r & 0x3f) | 0x80;
		r = r >> 6;
		oc.data[0] = r | 0xe0;
	}
	else if(r <= utf8_range_4byte){
		// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		oc.data[3] = (r & 0x3f) | 0x80;
		r = r >> 6;
		oc.data[2] = (r & 0x3f) | 0x80;
		r = r >> 6;
		oc.data[1] = (r & 0x3f) | 0x80;
		r = r >> 6;
		oc.data[0] = r | 0xf0;
	}

	return oc;
}

// Decode octet to code point
rune decode_octet(Octet oc){
	rune r = 0;
	uint len = octet_len(oc.data[0]);
	if(len == 1){
		// 0xxxxxxx
		r = oc.data[0];
	} else if(len == 2){
		// 110xxxxx 10xxxxxx
		r  = (oc.data[1] & 0x3f);
		r |= (oc.data[0] & 0x1f) << 6;
	} else if(len == 3){
		// 1110xxxx 10xxxxxx 10xxxxxx
		r  = (oc.data[2] & 0x3f);
		r |= (oc.data[1] & 0x3f) << 6;
		r |= (oc.data[0] & 0x0f) << 12;
	} else if(len == 4){
		// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		r  = (oc.data[3] & 0x3f);
		r |= (oc.data[2] & 0x3f) << 6;
		r |= (oc.data[1] & 0x3f) << 12;
		r |= (oc.data[1] & 0x07) << 18;
	}
	return r;
}

// return 0
// 	+ (((b >> 5) == 0x06) * 2)
// 	+ (((b >> 4) == 0x0e) * 3)
// 	+ (((b >> 3) == 0x1e) * 4);
