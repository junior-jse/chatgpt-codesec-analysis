#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char find_most_common_char(const char *str, size_t *max_count) {
    size_t counts[UCHAR_MAX + 1] = {0};
    /* Possible weaknesses found:
     *  Null pointer dereference
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     *  Either the condition 'str==NULL' is redundant or there is possible null pointer dereference: str. [nullPointerRedundantCheck]
     */
    size_t length = strlen(str);
    size_t current_max = 0;
    char most_common = '\0';
    size_t i;

    /* Possible weaknesses found:
     *  Assuming that condition 'str==NULL' is not redundant
     */
    if (str == NULL || max_count == NULL) {
        return '\0';
    }

    for (i = 0; i < length; i++) {
        unsigned char index = (unsigned char)str[i];
        counts[index]++;
    }

    for (i = 0; i < UCHAR_MAX + 1; i++) {
        if (counts[i] > current_max) {
            current_max = counts[i];
            most_common = (char)i;
        }
    }

    *max_count = current_max;
    return most_common;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t count = 0;
    char result = find_most_common_char(argv[1], &count);

    if (count > 0) {
        printf("Most common character: '%c' (Count: %zu)\n", result, count);
    } else {
        printf("String is empty.\n");
    }

    return EXIT_SUCCESS;
}