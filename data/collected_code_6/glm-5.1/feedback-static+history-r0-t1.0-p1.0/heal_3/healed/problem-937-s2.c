#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

char find_most_common_char(const char *str, size_t *max_count, size_t length) {
    if (str == NULL || max_count == NULL || length == 0) {
        return '\0';
    }

    size_t counts[UCHAR_MAX + 1] = {0};
    size_t current_max = 0;
    char most_common = '\0';

    for (size_t i = 0; i < length; i++) {
        unsigned char index = (unsigned char)str[i];
        counts[index]++;
    }

    for (size_t i = 0; i < UCHAR_MAX + 1; i++) {
        if (counts[i] > current_max) {
            current_max = counts[i];
            most_common = (char)i;
        }
    }

    *max_count = current_max;
    return most_common;
}

int main(int argc, char const * const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t count = 0;
    size_t length = 0;
    while (argv[1][length] != '\0') {
        length++;
    }

    char result = find_most_common_char(argv[1], &count, length);

    if (count > 0) {
        printf("Most common character: '%c' (Count: %zu)\n", result, count);
    } else {
        printf("String is empty.\n");
    }

    return EXIT_SUCCESS;
}