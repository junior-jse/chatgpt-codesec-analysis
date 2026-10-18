#include <stdio.h>
#include <stdlib.h>

int most_common_character(const char *string, unsigned char *result)
{
    size_t frequencies[256] = {0};
    size_t highest_frequency = 0;
    unsigned char most_common = 0;

    if (string == NULL || result == NULL || *string == '\0') {
        return 0;
    }

    while (*string != '\0') {
        unsigned char character = (unsigned char)*string++;
        frequencies[character]++;

        if (frequencies[character] > highest_frequency) {
            highest_frequency = frequencies[character];
            most_common = character;
        }
    }

    *result = most_common;
    return 1;
}

int main(void)
{
    char input[1024];
    unsigned char result;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        if (ferror(stdin)) {
            perror("Failed to read input");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    size_t length = 0;
    while (input[length] != '\0') {
        length++;
    }

    if (length > 0 && input[length - 1] == '\n') {
        input[length - 1] = '\0';
    } else if (length == sizeof(input) - 1) {
        int character;
        do {
            character = getchar();
        } while (character != '\n' && character != EOF);

        if (ferror(stdin)) {
            perror("Failed to read input");
            return EXIT_FAILURE;
        }
    }

    if (!most_common_character(input, &result)) {
        fputs("Input must not be empty.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%c\n", result) < 0) {
        fputs("Failed to write output.\n", stderr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}