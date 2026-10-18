#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *remove_character(const char *input, char character)
{
    size_t input_length;
    size_t output_length = 0;
    char *output;
    size_t i;
    size_t j = 0;

    if (input == NULL) {
        return NULL;
    }

    input_length = strlen(input);

    for (i = 0; i < input_length; ++i) {
        if (input[i] != character) {
            ++output_length;
        }
    }

    output = malloc(output_length + 1);
    if (output == NULL) {
        return NULL;
    }

    for (i = 0; i < input_length; ++i) {
        if (input[i] != character) {
            output[j++] = input[i];
        }
    }

    output[j] = '\0';
    return output;
}

int main(void)
{
    char input[1024];
    char character;
    char *result;

    if (fgets(input, sizeof input, stdin) == NULL) {
        return EXIT_FAILURE;
    }

    input[strcspn(input, "\n")] = '\0';

    if (scanf(" %c", &character) != 1) {
        return EXIT_FAILURE;
    }

    result = remove_character(input, character);
    if (result == NULL) {
        return EXIT_FAILURE;
    }

    if (puts(result) == EOF) {
        free(result);
        return EXIT_FAILURE;
    }

    free(result);
    return EXIT_SUCCESS;
}