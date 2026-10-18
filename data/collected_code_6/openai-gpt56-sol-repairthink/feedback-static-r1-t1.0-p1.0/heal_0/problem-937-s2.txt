#include <limits.h>
#include <stddef.h>
#include <stdio.h>

int most_common_character(const char *string, unsigned char *result, size_t *count)
{
    size_t frequencies[UCHAR_MAX + 1] = {0};
    size_t highest = 0;
    unsigned char most_common = 0;

    if (string == NULL || result == NULL || count == NULL || *string == '\0') {
        return -1;
    }

    while (*string != '\0') {
        unsigned char character = (unsigned char)*string++;
        frequencies[character]++;

        if (frequencies[character] > highest) {
            highest = frequencies[character];
            most_common = character;
        }
    }

    *result = most_common;
    *count = highest;
    return 0;
}

int main(void)
{
    char input[4096];
    unsigned char character;
    size_t count;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        fputs("Failed to read input.\n", stderr);
        return 1;
    }

    size_t length = 0;
    while (input[length] != '\0') {
        length++;
    }

    if (length > 0 && input[length - 1] == '\n') {
        input[length - 1] = '\0';
    } else if (length == sizeof(input) - 1) {
        int next_character = getchar();

        if (next_character != '\n' && next_character != EOF) {
            while ((next_character = getchar()) != '\n' && next_character != EOF) {
            }

            fputs("Input is too long.\n", stderr);
            return 1;
        }

        if (ferror(stdin)) {
            fputs("Failed to read input.\n", stderr);
            return 1;
        }
    }

    if (most_common_character(input, &character, &count) != 0) {
        fputs("Input must not be empty.\n", stderr);
        return 1;
    }

    if (printf("%c %zu\n", character, count) < 0) {
        fputs("Failed to write output.\n", stderr);
        return 1;
    }

    return 0;
}