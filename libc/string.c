#include "string.h"
#include "memory.h"

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}

size_t strnlen(const char *str, size_t count) {
  size_t len = 0;
  while (str[len] && count > 0) {
    len++;
    count--;
  }
  return len;
}

int strncmp(const char *str1, const char *str2, size_t count) {
  char c1 = '\0', c2 = '\0';
  while (count-- > 0) {
    c1 = *str1++;
    c2 = *str2++;
    if (c1 == '\0' || c1 != c2)
      return ((int)c1) - ((int)c2);
  }
  return ((int)c1) - ((int)c2);
}

char *strncpy(char *dst, const char *src, size_t count) {
  size_t size = strnlen(src, count);
  if (size != count)
    memset(dst + size, '\0', count - size);
  dst[size] = '\0';
  return memcpy(dst, src, size);
}

char *strncat(char *dst, const char *src, size_t count) {
  char *s = dst;
  dst += strlen(dst);
  size_t size = strnlen(src, count);
  dst[size] = '\0';
  memcpy(dst, src, size);
  return s;
}

inline bool isdigit(char c) { return ('0' <= c) && (c <= '9'); }

inline bool isxdigit(char c) {
  return (('0' <= c) && (c <= '9')) || (('A' <= c) && (c <= 'F')) ||
         (('a' <= c) && (c <= 'f'));
}

inline bool isupper(char c) { return ('A' <= c) && (c <= 'Z'); }

inline bool islower(char c) { return ('a' <= c) && (c <= 'z'); }

inline bool isalpha(char c) { return islower(c) || isupper(c); }

inline bool isspace(char c) {
  return (c == 0x20) || (c == 0x0c) || (c == 0x0a) || (c == 0x0d) ||
         (c == 0x09) || (c == 0x0b);
}
