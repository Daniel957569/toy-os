#include "utils.h"

void memory_copy(char *source, char *dest, int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
}

void reverseString(char *str) {
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

    reverseString(str);
    str[i] = '\0';
}
