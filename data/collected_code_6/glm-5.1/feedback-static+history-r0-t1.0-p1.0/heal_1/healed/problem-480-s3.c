#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

char find_max_occurring_char(const char *str) {
    int counts[UCHAR_MAX + 1] = {0};
    size_t len = 0;
    const char *p = str;
    while (*p != '\0') {
        len++;
        p++;
    }

    for (size_t i = 0; i < len; i++) {
        unsigned char c = str[i];
        counts[c]++;
    }

    int max_count = 0;
    char max_char = '\0';

    for (size_t i = 0; i <= UCHAR_MAX; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            max_char = (char)i;
        }
    }

    return max_char;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argv[1][0] == '\0') {
        fprintf(stderr, "Error: Input string cannot be empty.\n");
        return EXIT_FAILURE;
    }

    char result = find_max_occurring_char(argv[1]);
    printf("%c\n", result);

    return EXIT_SUCCESS;
}