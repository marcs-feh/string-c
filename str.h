#ifndef INCLUDE_STR_H_
#define INCLUDE_STR_H_

#include "types.h"
#include "utf8.h"

// Sequence of bytes
typedef struct {
	usize size;
	byte* buf;
} Byte_Seq;

typedef struct {
	usize size;    // String lenght, not including NUL terminator.
	Byte_Seq data; // String storage, UTF-8 bytes.
} String;

Byte_Seq byte_seq_new(usize n);
void     byte_seq_del(Byte_Seq* seq);
void     byte_seq_resize(Byte_Seq* seq, usize n);

String string_new(usize size_hint);
String string_from_cstr(const char* cs);
void   string_del(String* s);
void   string_copy(String* dest, String* src);
void   string_move(String* dest, String* src);
void   string_append_byte(String* s, byte b);
// void string_append_rune(String* s, byte b);

#endif /* include guard */
