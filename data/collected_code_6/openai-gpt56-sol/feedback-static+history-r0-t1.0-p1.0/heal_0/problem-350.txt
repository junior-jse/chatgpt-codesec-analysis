#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *remove_character(const char *input, char character)
{
    size_t input_length;
    size_t output_length = 0;
    size_t index;
    char *output;

    if (input == NULL) {
        return NULL;
    }

    input_length = strlen(input);

    for (index = 0; index < input_length; ++index) {
        if (input[index] != character) {
            ++output_length;
        }
    }

    output = malloc(output_length + 1);
    if (output == NULL) {
        return NULL;
    }

    output_length = 0;

    for (index = 0; index < input_length; ++index) {
        if (input[index] != character) {
            output[output_length++] = input[index];
        }
    }

    output[output_length] = '\0';
    return output;
}

int main(void)
{
    char input[4096];
    char character_line[4096];
    char *result;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    input[strcspn(input, "\n")] = '\0';

    if (fgets(character_line, sizeof(character_line), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    if (character_line[0] == '\0' || character_line[0] == '\n') {
        return EXIT_FAILURE;
    }

    result = remove_character(input, character_line[0]);
    if (result == NULL) {
        return EXIT_FAILURE;
    }

    if (printf("%s\n", result) < 0) {
        free(result);
        return EXIT_FAILURE;
    }

    free(result);
    return EXIT_SUCCESS;
}