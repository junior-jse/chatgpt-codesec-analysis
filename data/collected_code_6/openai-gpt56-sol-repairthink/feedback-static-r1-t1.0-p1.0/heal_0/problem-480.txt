#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int find_max_occurring_character(const char *str, unsigned char *result)
{
    size_t frequencies[UCHAR_MAX + 1] = {0};
    size_t max_count = 0;

    if (str == NULL || result == NULL || *str == '\0') {
        return 0;
    }

    for (const unsigned char *p = (const unsigned char *)str; *p != '\0'; ++p) {
        ++frequencies[*p];
    }

    for (size_t i = 0; i <= UCHAR_MAX; ++i) {
        if (frequencies[i] > max_count) {
            max_count = frequencies[i];
            *result = (unsigned char)i;
        }
    }

    return 1;
}

int main(void)
{
    char input[1024];
    unsigned char result;

    if (fgets(input, sizeof input, stdin) == NULL) {
        fputs("Failed to read input.\n", stderr);
        return EXIT_FAILURE;
    }

    size_t length = 0;
    while (input[length] != '\0' && input[length] != '\n') {
        ++length;
    }

    if (input[length] == '\n') {
        input[length] = '\0';
    } else if (input[length] != '\0') {
        fputs("Input is too long.\n", stderr);
        return EXIT_FAILURE;
    }

    if (!find_max_occurring_character(input, &result)) {
        fputs("Input must not be empty.\n", stderr);
        return EXIT_FAILURE;
    }

    printf("%c\n", result);
    return EXIT_SUCCESS;
}