#ifndef STRING_H
#define STRING_H

#include "../common.h"

void int_to_ascii(int n, char *str);
void reverse_string(char *s);
int strlen(char *s);
void append(char *dest, char c);
void pop_char(char* str);
int strcmp(char* s1, char* s2);
void clear_string(char* str);


#endif
