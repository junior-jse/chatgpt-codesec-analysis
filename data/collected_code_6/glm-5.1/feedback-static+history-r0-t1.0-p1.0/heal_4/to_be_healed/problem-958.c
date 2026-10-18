#include <stdio.h>
#include <stdlib.h>

char* int_to_roman(int num) {
    if (num < 1 || num > 3999) {
        return NULL;
    }

    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char* const numerals[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};

    char* result = malloc(16);
    if (result == NULL) {
        return NULL;
    }

    int idx = 0;
    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            num -= values[i];
            const char* sym = numerals[i];
            while (*sym) {
                result[idx++] = *sym++;
            }
        }
    }
    result[idx] = '\0';
    return result;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char* const argv[const]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer>\n", argv[0]);
        return 1;
    }

    char* endptr;
    long val = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || val < 1 || val > 3999) {
        fprintf(stderr, "Error: Input must be an integer between 1 and 3999\n");
        return 1;
    }

    char* roman = int_to_roman((int)val);
    if (roman == NULL) {
        fprintf(stderr, "Error: Memory allocation failed or invalid input\n");
        return 1;
    }

    printf("%s\n", roman);
    free(roman);
    return 0;
}