#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* int_to_roman(int num) {
    if (num <= 0 || num > 3999) {
        return NULL;
    }

    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const size_t sym_lengths[] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1};
    const char* const symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};

    size_t max_len = 16;
    char* result = (char*)malloc(max_len * sizeof(char));
    if (result == NULL) {
        return NULL;
    }

    size_t offset = 0;

    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            size_t sym_len = sym_lengths[i];
            if (offset + sym_len >= max_len) {
                size_t new_max_len = max_len * 2;
                char* new_result = (char*)realloc(result, new_max_len * sizeof(char));
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
                max_len = new_max_len;
            }
            memcpy(result + offset, symbols[i], sym_len);
            offset += sym_len;
            num -= values[i];
        }
    }

    result[offset] = '\0';

    return result;
}

int main(int argc, char* const argv[]) {
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