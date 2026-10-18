#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* int_to_roman(int num) {
    if (num <= 0 || num > 3999) {
        return NULL;
    }

    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char* symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    const size_t sym_lens[] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1};

    size_t buf_size = 16;
    char* result = malloc(buf_size);
    if (result == NULL) {
        return NULL;
    }

    size_t len = 0;

    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            size_t sym_len = sym_lens[i];
            
            while (len + sym_len + 1 > buf_size) {
                buf_size *= 2;
                char* new_result = realloc(result, buf_size);
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
            }
            
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + len, symbols[i], sym_len);
            len += sym_len;
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