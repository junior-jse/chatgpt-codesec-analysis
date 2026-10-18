#include <stdio.h>
#include <stdlib.h>

char *int_to_roman(int num) {
    if (num <= 0 || num > 3999) {
        return NULL;
    }

    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char *symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};

    char *result = malloc(16);
    if (result == NULL) {
        return NULL;
    }

    char *p = result;
    size_t remaining = 16;
    int i = 0;

    while (num > 0) {
        while (num >= values[i]) {
            int written = snprintf(p, remaining, "%s", symbols[i]);
            p += written;
            remaining -= written;
            num -= values[i];
        }
        i++;
    }

    return result;
}

int main(void) {
    int test_values[] = {1, 3, 4, 9, 40, 90, 400, 900, 1994, 2023, 3999, 0, -5, 4000};
    int num_tests = sizeof(test_values) / sizeof(test_values[0]);

    for (int i = 0; i < num_tests; i++) {
        printf("%d -> ", test_values[i]);
        char *roman = int_to_roman(test_values[i]);
        if (roman != NULL) {
            printf("%s\n", roman);
            free(roman);
        } else {
            printf("Invalid input (must be 1-3999)\n");
        }
    }

    return 0;
}