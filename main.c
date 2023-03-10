#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "utf8.h"
#include "str.h"

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

int main(int argc, const char** argv){
	rune point = '$';
	Octet oc = encode_rune(point);
	if(!octet_validate(oc)){ printf("Invalid octet\n"); }
	print_octet(oc);

	point = 0x00a3;
	oc    = encode_rune(point);
	if(!octet_validate(oc)){ printf("Invalid octet\n"); }
	print_octet(oc);

	point = 0xd55c;
	oc    = encode_rune(point);
	if(!octet_validate(oc)){ printf("Invalid octet\n"); }
	print_octet(oc);

	point = 0x10348;
	oc    = encode_rune(point);
	if(!octet_validate(oc)){ printf("Invalid octet\n"); }
	print_octet(oc);

	String s1 = string_from_cstr("Le ");
	string_append_rune(&s1, 0x65e5); // Nihongo
	string_append_rune(&s1, 0x672c);
	string_append_rune(&s1, 0x8a9e);
	string_append_cstr(&s1, " is quite pog");
	printf("'%s', runes:%zu, bytes:%zu, ok:%d\n", s1.data.buf, string_len(&s1), s1.size, string_validate(&s1));
	// for(usize idx = 0; idx < count_runes(&s1); idx += 1){
	// 	printf("str[%zu] = %x\n", idx, string_at(&s1, idx));
	// 	printf("str[%zu] -> %zu\n", idx, string_rune_byte_pos(&s1, idx));
	// }
	string_del(&s1);
	return 0;
}
