#include "types.h"

typedef struct {
	u32  size;
	byte data[4];
} Octet;

const u32 utf8_range_1byte = 0x0000007f;
const u32 utf8_range_2byte = 0x000007ff;
const u32 utf8_range_3byte = 0x0000ffff;
const u32 utf8_range_4byte = 0x0010ffff;

Octet encode_rune(rune r){
	Octet oc = {0};
	if(r <= utf8_range_1byte){
		// 0xxxxxxx
		oc.size = 1;
		oc.data[0] = 0x7f & r; // Set most signficant bit to 0
	}
	else if(r <= utf8_range_2byte){
		// 110xxxxx 10xxxxxx
		oc.size = 2;
		oc.data[1] = (r & 0x3f) | 0x80;
		r = r >> 6;
		oc.data[0] = (r & 0x1f) | 0xc0; // TODO eliminate &?
	}
	else if(r <= utf8_range_3byte){
		// 0x1110xxxx 10xxxxxx 10xxxxxx
		oc.size = 3;
		oc.data[2] = (r & 0x3f) | 0x80;
		r = r >> 6;
		oc.data[1] = (r & 0x3f) | 0x80;
		r = r >> 6;
		oc.data[0] = (r & 0x0f) | 0xe0; // TODO eliminate &?
	}
	else if(r <= utf8_range_4byte){
		// 0x11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		oc.size = 4;
		oc.data[3] = (r & 0x3f) | 0x80;
		r = r >> 6;
		oc.data[2] = (r & 0x3f) | 0x80;
		r = r >> 6;
		oc.data[1] = (r & 0x3f) | 0x80;
		r = r >> 6;
		oc.data[0] = (r & 0x07) | 0xf0; // TODO eliminate &?
	}

	return oc;
}

#include <stdio.h>

void byte_as_bin_str(char buf[8], byte b){
	for(uint i = 0; i < 8; i += 1){
		buf[8 - (i + 1)] = (b >> i) & 1 ? '1' : '0';
	}
}

void print_octet(Octet oc){
	char buf[9];
	buf[8] = '\0';
	if(oc.size >= 1){
		byte_as_bin_str(buf, oc.data[0]);
		printf("%s ", buf);
	}
	if(oc.size >= 2){
		byte_as_bin_str(buf, oc.data[1]);
		printf("%s ", buf);
	}
	if(oc.size >= 3){
		byte_as_bin_str(buf, oc.data[2]);
		printf("%s ", buf);
	}
	if(oc.size >= 4){
		byte_as_bin_str(buf, oc.data[3]);
		printf("%s ", buf);
	}
	printf("\n");
}
