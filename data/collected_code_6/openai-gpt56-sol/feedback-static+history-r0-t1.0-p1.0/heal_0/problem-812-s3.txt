#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *abbreviate_road(const char *input)
{
    static const char target[] = "road";
    static const char replacement[] = "rd.";
    size_t input_length;
    size_t count = 0;
    size_t output_length;
    const char *cursor;
    const char *match;
    char *output;
    char *destination;

    if (input == NULL) {
        return NULL;
    }

    input_length = strlen(input);
    cursor = input;

    while ((match = strstr(cursor, target)) != NULL) {
        ++count;
        cursor = match + sizeof(target) - 1;
    }

    output_length = input_length - count * ((sizeof(target) - 1) - (sizeof(replacement) - 1));

    if (output_length == SIZE_MAX) {
        return NULL;
    }

    output = malloc(output_length + 1);
    if (output == NULL) {
        return NULL;
    }

    cursor = input;
    destination = output;

    while ((match = strstr(cursor, target)) != NULL) {
        size_t prefix_length = (size_t)(match - cursor);

        memcpy(destination, cursor, prefix_length);
        destination += prefix_length;

        memcpy(destination, replacement, sizeof(replacement) - 1);
        destination += sizeof(replacement) - 1;

        cursor = match + sizeof(target) - 1;
    }

    strcpy(destination, cursor);
    return output;
}

int main(void)
{
    char input[1024];
    char *result;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        if (ferror(stdin)) {
            fprintf(stderr, "Failed to read input.\n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    input[strcspn(input, "\n")] = '\0';

    result = abbreviate_road(input);
    if (result == NULL) {
        fprintf(stderr, "Failed to abbreviate input.\n");
        return EXIT_FAILURE;
    }

    if (puts(result) == EOF) {
        free(result);
        fprintf(stderr, "Failed to write output.\n");
        return EXIT_FAILURE;
    }

    free(result);
    return EXIT_SUCCESS;
}