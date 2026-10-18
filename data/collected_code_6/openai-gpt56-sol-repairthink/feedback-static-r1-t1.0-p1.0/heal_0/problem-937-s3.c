#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int most_common_character(const char *string, unsigned char *character, size_t *count)
{
    size_t frequencies[256] = {0};
    size_t max_count = 0;
    unsigned char most_common = 0;

    if (string == NULL || character == NULL || count == NULL || *string == '\0') {
        return -1;
    }

    for (const unsigned char *p = (const unsigned char *)string; *p != '\0'; ++p) {
        ++frequencies[*p];
    }

    for (size_t i = 0; i < 256; ++i) {
        if (frequencies[i] > max_count) {
            max_count = frequencies[i];
            most_common = (unsigned char)i;
        }
    }

    *character = most_common;
    *count = max_count;
    return 0;
}

int main(void)
{
    char input[1024];
    unsigned char character;
    size_t count;

    if (fgets(input, sizeof input, stdin) == NULL) {
        fputs("Failed to read input.\n", stderr);
        return EXIT_FAILURE;
    }

    size_t length = strlen(input);

    if (length > 0 && input[length - 1] == '\n') {
        input[length - 1] = '\0';
    } else if (!feof(stdin)) {
        int ch;

        do {
            ch = getchar();
        } while (ch != '\n' && ch != EOF);

        fputs("Input is too long.\n", stderr);
        return EXIT_FAILURE;
    }

    if (most_common_character(input, &character, &count) != 0) {
        fputs("Input must not be empty.\n", stderr);
        return EXIT_FAILURE;
    }

    printf("Most common character: '%c'\nCount: %zu\n", character, count);
    return EXIT_SUCCESS;
}