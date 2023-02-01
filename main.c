#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "utf8.c"

int main(int argc, const char** argv){
	rune point = '$';
	Octet oc = encode_rune(point);
	printf("$\t ");
	print_octet(oc);

	point = 0x00a3;
	oc = encode_rune(point);
	printf("£\t ");
	print_octet(oc);

	point = 0xd55c;
	oc = encode_rune(point);
	printf("한\t ");
	print_octet(oc);

	point = 0x10348;
	oc = encode_rune(point);
	printf("𐍈\t");
	print_octet(oc);
	return 0;
}
