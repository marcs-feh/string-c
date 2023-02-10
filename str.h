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
	usize size;    // String lenght in bytes, not including NUL terminator.
	Byte_Seq data; // String storage, UTF-8 bytes.
} String;

Byte_Seq byte_seq_new(usize n);
void     byte_seq_del(Byte_Seq* seq);
void     byte_seq_resize(Byte_Seq* seq, usize n);

// Create a new string with size_hint pre-allocated bytes (using 0 is fine.)
String string_new(usize size_hint);
// Form a string from a C-string
String string_from_cstr(const char* cs);
// De-allocate a string
void   string_del(String* s);
// Copy one string to another
void   string_copy(String* dest, String* src);
// Move storage of one string to another
void   string_move(String* dest, String* src);
// Get the n-th rune of a string (returns 0 for invalid index)
rune   string_at(String* s, usize idx);
// String lenght (in runes)
usize  string_len(String* s);
// Get the n-th byte of a string (returns 0 for invalid index)
byte   string_byte_at(String* s, usize idx);
// Get position (byte offset) of the n-th rune (returns 0 for invalid index)
usize  string_rune_byte_pos(String* s, usize idx);
// Append a byte (or ASCII char) to string
void   string_append_byte(String* s, byte b);
// Append a rune (Unicode codepoint)
void   string_append_rune(String* s, rune b);
// Append a C-string
void   string_append_cstr(String* s, const char* cs);
// Append one string to another
void   string_append_str(String* dest, String* src);
// Validate string.
bool   string_validate(const String* s);

#endif /* include guard */
