#include <stdio.h>
#include <stdlib.h>

char* int_to_roman(int num) {
    if (num <= 0 || num > 3999) {
        return NULL;
    }

    /* Possible weaknesses found:
     *  Variable 'values' can be declared as const array [constVariable]
     */
    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    char* symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};

    char* result = (char*)malloc(16 * sizeof(char));
    if (result == NULL) {
        return NULL;
    }

    int idx = 0;
    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            num -= values[i];
            result[idx++] = symbols[i][0];
            if (symbols[i][1] != '\0') {
                result[idx++] = symbols[i][1];
            }
        }
    }

    result[idx] = '\0';
    return result;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer>\n", argv[0]);
        return 1;
    }

    /* Possible weaknesses found:
     * Flawfinder atoi: Unless checked, the resulting number can exceed the expected range (CWE-190). If source untrusted, check both minimum and maximum, even if the input had no minus sign (large numbers can roll over into negative number; consider saving to an unsigned value if that is intended). (risk 2, integer)
     */
    int num = atoi(argv[1]);
    char* roman = int_to_roman(num);

    if (roman == NULL) {
        fprintf(stderr, "Invalid input or memory allocation failed. Integer must be between 1 and 3999.\n");
        return 1;
    }

    printf("%d -> %s\n", num, roman);
    
    free(roman);
    roman = NULL;

    return 0;
}