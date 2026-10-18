#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *camel_to_snake(const char *input)
{
    size_t input_length;
    size_t output_length = 0;
    size_t i;
    size_t j = 0;
    char *output;

    if (input == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    input_length = strlen(input);

    for (i = 0; i < input_length; ++i) {
        unsigned char current = (unsigned char)input[i];

        if (isupper(current) && i > 0) {
            unsigned char previous = (unsigned char)input[i - 1];
            unsigned char next = i + 1 < input_length
                                     ? (unsigned char)input[i + 1]
                                     : 0;

            if (islower(previous) || isdigit(previous) ||
                (isupper(previous) && next != 0 && islower(next))) {
                if (output_length == SIZE_MAX) {
                    return NULL;
                }
                ++output_length;
            }
        }

        if (output_length == SIZE_MAX) {
            return NULL;
        }
        ++output_length;
    }

    if (output_length == SIZE_MAX) {
        return NULL;
    }

    output = malloc(output_length + 1);
    if (output == NULL) {
        return NULL;
    }

    for (i = 0; i < input_length; ++i) {
        unsigned char current = (unsigned char)input[i];

        if (isupper(current) && i > 0) {
            unsigned char previous = (unsigned char)input[i - 1];
            unsigned char next = i + 1 < input_length
                                     ? (unsigned char)input[i + 1]
                                     : 0;

            if (islower(previous) || isdigit(previous) ||
                (isupper(previous) && next != 0 && islower(next))) {
                output[j++] = '_';
            }
        }

        output[j++] = (char)tolower(current);
    }

    output[j] = '\0';
    return output;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[])
{
    char *snake_case;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <camelCaseString>\n", argv[0]);
        return EXIT_FAILURE;
    }

    snake_case = camel_to_snake(argv[1]);
    if (snake_case == NULL) {
        fputs("Conversion failed\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s\n", snake_case) < 0) {
        free(snake_case);
        return EXIT_FAILURE;
    }

    free(snake_case);
    return EXIT_SUCCESS;
}