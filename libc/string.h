#ifndef STRING_H
#define STRING_H

#include "common.h"

size_t strlen(const char *str);

size_t strnlen(const char *str, size_t count);
int strncmp(const char *str1, const char *str2, size_t count);
char *strncpy(char *dst, const char *src, size_t count);
char *strncat(char *dst, const char *src, size_t count);

bool isdigit(char c);
bool isxdigit(char c);

bool isalpha(char c);
bool islower(char c);
bool isupper(char c);

bool isspace(char c);

#endif
