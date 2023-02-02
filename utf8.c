#include "types.h"

typedef struct {
	byte data[4];
} Octet;

const u32 utf8_range_1byte = 0x0000007f;
const u32 utf8_range_2byte = 0x000007ff;
const u32 utf8_range_3byte = 0x0000ffff;
const u32 utf8_range_4byte = 0x0010ffff;

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

#include <stdio.h>

void byte_as_bin_str(char buf[8], byte b){
	for(uint i = 0; i < 8; i += 1){
		buf[8 - (i + 1)] = (b >> i) & 1 ? '1' : '0';
	}
}

void print_octet(Octet oc){
	char buf[9];
	byte databuf[5];
	uint len = octet_len(oc.data[0]);
	for(uint i = 0; i < 4; i += 1){
		databuf[i] = oc.data[i];
	}
	buf[8] = '\0';

	switch(len){
		case 1: databuf[1] = '\0'; break;
		case 2: databuf[2] = '\0'; break;
		case 3: databuf[3] = '\0'; break;
		case 4: databuf[4] = '\0'; break;
	}
	printf("%s\t| %8.2x\t| ", databuf, decode_octet(oc));

	if(len >= 1){
		byte_as_bin_str(buf, oc.data[0]);
		printf("%s ", buf);
	}
	if(len >= 2){
		byte_as_bin_str(buf, oc.data[1]);
		printf("%s ", buf);
	}
	if(len >= 3){
		byte_as_bin_str(buf, oc.data[2]);
		printf("%s ", buf);
	}
	if(len >= 4){
		byte_as_bin_str(buf, oc.data[3]);
		printf("%s\n", buf);
	}
	printf("\n");
}

// return 0
// 	+ (((b >> 5) == 0x06) * 2)
// 	+ (((b >> 4) == 0x0e) * 3)
// 	+ (((b >> 3) == 0x1e) * 4);
