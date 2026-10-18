#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char count_most_common_char(const char *str, size_t *out_count) {
    size_t counts[UCHAR_MAX + 1] = {0};
    size_t max_count = 0;
    char max_char = '\0';

    if (str == NULL || out_count == NULL) {
        return '\0';
    }

    for (size_t i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        counts[c]++;
    }

    for (size_t i = 0; i <= UCHAR_MAX; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            max_char = (char)i;
        }
    }

    *out_count = max_count;
    return max_char;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char * const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t count = 0;
    char result = count_most_common_char(argv[1], &count);

    if (count > 0) {
        printf("Most common character: '%c' (Count: %zu)\n", result, count);
    } else {
        printf("String is empty.\n");
    }

    return EXIT_SUCCESS;
}