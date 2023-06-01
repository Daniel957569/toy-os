#include "string.h"

void reverse_string(char *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }

    int start = 0;
    int end = length - 1;

    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        start++;
        end--;
    }
}
void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0)
        str[i++] = '-';
    str[i] = '\0';
    reverse_string(str);
}

int strlen(char *s) {
    int i = 0;
    while (s[i] != '\0')
        i++;
    return i;
}

void pop_char(char *str) {
    int len = strlen(str);
    str[len] = '\0';
}

void append(char *dest, char c) {
    int len = strlen(dest);
    dest[len] = c;
    dest[len + 1] = '\0';
}

int strcmp(char *s1, char *s2) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0')
            return 0;
    }
    return s1[i] - s2[i];
}

void clear_string(char *str) { str[0] = '\0'; }
