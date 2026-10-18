#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* int_to_roman(int num) {
    if (num <= 0 || num > 3999) {
        return NULL;
    }

    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char* symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};

    char* result = (char*)malloc(16 * sizeof(char));
    if (result == NULL) {
        return NULL;
    }

    size_t len = 0;
    size_t buf_size = 16;

    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            size_t sym_len = strlen(symbols[i]);
            if (len + sym_len < buf_size) {
                memcpy(result + len, symbols[i], sym_len);
                len += sym_len;
            }
            num -= values[i];
        }
    }
    result[len] = '\0';

    return result;
}

int main(int argc, const char* const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer>\n", argv[0]);
        return 1;
    }

    char* endptr;
    long val = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || val < 1 || val > 3999) {
        fprintf(stderr, "Error: Input must be an integer between 1 and 3999.\n");
        return 1;
    }

    char* roman = int_to_roman((int)val);

    if (roman == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 1;
    }

    printf("%s\n", roman);

    free(roman);

    return 0;
}