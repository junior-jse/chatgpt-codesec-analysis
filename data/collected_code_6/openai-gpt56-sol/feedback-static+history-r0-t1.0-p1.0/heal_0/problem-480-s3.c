#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int find_max_occurring_character(const char *str, unsigned char *result)
{
    size_t frequencies[UCHAR_MAX + 1] = {0};
    size_t max_count = 0;
    unsigned int max_character = 0;

    if (str == NULL || result == NULL || *str == '\0') {
        return 0;
    }

    for (const unsigned char *p = (const unsigned char *)str; *p != '\0'; ++p) {
        ++frequencies[*p];
    }

    for (unsigned int i = 0; i <= UCHAR_MAX; ++i) {
        if (frequencies[i] > max_count) {
            max_count = frequencies[i];
            max_character = i;
        }
    }

    *result = (unsigned char)max_character;
    return 1;
}

int main(void)
{
    char input[4096];
    unsigned char result;

    if (fgets(input, sizeof input, stdin) == NULL) {
        if (ferror(stdin)) {
            perror("Failed to read input");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    size_t length = 0;
    while (input[length] != '\0') {
        ++length;
    }

    if (length > 0 && input[length - 1] == '\n') {
        input[length - 1] = '\0';
    } else if (length == sizeof input - 1) {
        int ch;

        do {
            ch = getchar();
        } while (ch != '\n' && ch != EOF);

        if (ferror(stdin)) {
            perror("Failed to read input");
            return EXIT_FAILURE;
        }

        if (ch != '\n') {
            fputs("Input is too long.\n", stderr);
            return EXIT_FAILURE;
        }
    }

    if (!find_max_occurring_character(input, &result)) {
        fputs("Input must not be empty.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%c\n", result) < 0) {
        perror("Failed to write output");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}