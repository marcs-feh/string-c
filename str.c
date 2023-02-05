#include "str.h"
#include <stdlib.h>

Byte_Seq byte_seq_new(usize n){
	Byte_Seq seq = {
		.size = n,
		.buf  = NULL,
	};
	if(n > 0){
		seq.buf = malloc(sizeof(*seq.buf) * n);
	}
	return seq;
}

void byte_seq_resize(Byte_Seq* seq, usize n){
	if(seq == NULL){ return; }
	seq->buf  = realloc(seq->buf, sizeof(*seq->buf) * n);
	seq->size = n;
}

void byte_seq_del(Byte_Seq* seq){
	if(seq == NULL){ return; }
	free(seq->buf);
	seq->size = 0;
	seq->buf = NULL;
}

String string_new(usize size_hint){
	String s = {
		.size = 0,
		.data = byte_seq_new(size_hint),
	};
	return s;
}

static usize cstring_len(const char* cs){
	usize n = 0;
	while(cs[n] != '\0'){ n += 1; }
	return n;
}

String string_from_cstr(const char *cs){
	if(cs == NULL){ return string_new(0); }
	usize slen = cstring_len(cs);
	String s   = string_new((slen + 1) * 1.30);
	for(usize i = 0; i < slen; i += 1){
		s.data.buf[i] = cs[i];
	}
	s.size = slen;
	s.data.buf[slen] = '\0';
	return s;
}

void string_copy(String *dest, String *src){
	if((dest == NULL) || (src == NULL)){ return; }
	if(dest->data.size <= src->size){
		byte_seq_resize(&dest->data, src->size + 2);
	}
	for(usize i = 0; i < src->size; i += 1){
		dest->data.buf[i] = src->data.buf[i];
	}
	dest->size = src->size;
	dest->data.buf[dest->size] = '\0';
}

void string_move(String *dest, String *src){
	if((dest == NULL) || (src == NULL)){ return; }
	string_del(dest);
	*dest = *src;
	src->data = (Byte_Seq){.size = 0, .buf = NULL};
	src->size = 0;
}

void string_append_byte(String* s, byte b){
	if(b == '\0'){ return; } // Appending NUL is not allowed.
	if((s->size + 1) >= s->data.size){
		byte_seq_resize(&s->data, (s->data.size + 2) * 1.5);
	}
	s->data.buf[s->size] = b;
	s->size += 1;
	s->data.buf[s->size] = '\0';
}

rune string_at(String* s, usize idx){
	// TODO: potentially very unsafe?
	if(idx >= s->size) { return 0; }
	usize i = 0, n = 0;
	while(i < s->size){
		uint len = octet_len(s->data.buf[i]);
		if(n == idx){
			Octet oc;
			for(uint j = 0; j < len; j += 1){
				oc.data[j] = s->data.buf[i+j];
			}
			return decode_octet(oc);
		}
		i += len;
		n += 1;
	}
	return 0;
}

usize string_rune_byte_pos(String *s, usize idx){
	if(idx >= s->size) { return 0; }
	usize i = 0, n = 0;
	while(i < s->size){
		uint len = octet_len(s->data.buf[i]);
		if(n == idx){
			Octet oc;
			for(uint j = 0; j < len; j += 1){
				oc.data[j] = s->data.buf[i+j];
			}
			return i;
		}
		i += len;
		n += 1;
	}
	return 0;
}

byte string_byte_at(String* s, usize idx){
	if(idx >= s->size){ return 0; }
	return s->data.buf[idx];
}

void string_append_cstr(String *s, const char *cs){
	if(cs == NULL) { return; }
	usize len = cstring_len(cs);
	// TODO: optimize this
	for(usize i = 0; i < len; i += 1){
		string_append_byte(s, cs[i]);
	}
}

void string_append_str(String *dest, String *src){
	if((dest == NULL) || (src == NULL)){ return; }

	for(usize i = 0; i < src->size; i += 1){}
}

void string_append_rune(String* s, rune r){
	Octet oc = encode_rune(r);
	uint  n  = octet_len(oc.data[0]);
	for(uint i = 0; i < n; i += 1){
		string_append_byte(s, oc.data[i]);
	}
}

usize count_runes(String* s){
	usize n = 0;
	usize i = 0;
	while(i < s->size){
		uint len = octet_len(s->data.buf[i]);
		i += len;
		n += 1;
	}
	return n;
}

void string_del(String* s){
	if(s == NULL) { return; }
	byte_seq_del(&s->data);
	s->size = 0;
}

