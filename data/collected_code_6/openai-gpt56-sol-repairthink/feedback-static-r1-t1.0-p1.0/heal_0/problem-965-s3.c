#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *camel_to_snake(const char *input)
{
    size_t input_length;
    size_t uppercase_count = 0;
    size_t output_length;
    size_t i;
    size_t j = 0;
    char *output;

    if (input == NULL) {
        return NULL;
    }

    input_length = strlen(input);

    for (i = 0; i < input_length; ++i) {
        unsigned char current = (unsigned char)input[i];

        if (isupper(current) && i > 0) {
            ++uppercase_count;
        }
    }

    if (uppercase_count > SIZE_MAX - input_length - 1) {
        return NULL;
    }

    output_length = input_length + uppercase_count;
    output = malloc(output_length + 1);

    if (output == NULL) {
        return NULL;
    }

    for (i = 0; i < input_length; ++i) {
        unsigned char current = (unsigned char)input[i];

        if (isupper(current) && i > 0) {
            output[j++] = '_';
        }

        output[j++] = (char)tolower(current);
    }

    output[j] = '\0';
    return output;
}

int main(int argc, char *argv[])
{
    char *result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <camelCaseString>\n", argv[0]);
        return EXIT_FAILURE;
    }

    result = camel_to_snake(argv[1]);

    if (result == NULL) {
        fprintf(stderr, "Failed to convert the input string.\n");
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