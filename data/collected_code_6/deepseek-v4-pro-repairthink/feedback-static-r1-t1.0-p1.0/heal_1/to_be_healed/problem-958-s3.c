#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void int_to_roman(int num, char *result) {
    if (result == NULL) {
        return;
    }

    if (num <= 0 || num > 3999) {
        result[0] = '\0';
        return;
    }

    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char *symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    
    result[0] = '\0';
    int remaining = num;
    
    for (int i = 0; i < 13; i++) {
        while (remaining >= values[i]) {
            /* Possible weaknesses found:
             * Flawfinder strcat: Does not check for buffer overflows when concatenating to destination [MS-banned] (CWE-120). Consider using strcat_s, strncat, strlcat, or snprintf (warning: strncat is easily misused). (risk 4, buffer)
             */
            strcat(result, symbols[i]);
            remaining -= values[i];
        }
    }
}

int main(void) {
    int test_numbers[] = {1, 3, 4, 9, 58, 1994, 3999};
    int num_tests = sizeof(test_numbers) / sizeof(test_numbers[0]);
    
    for (int i = 0; i < num_tests; i++) {
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char roman[32];
        int_to_roman(test_numbers[i], roman);
        printf("%d = %s\n", test_numbers[i], roman);
    }
    
    return 0;
}