#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *remove_character(const char *input, char character)
{
    size_t input_length;
    size_t output_length = 0;
    size_t i;
    char *output;

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

    output_length = 0;

    for (i = 0; i < input_length; ++i) {
        if (input[i] != character) {
            output[output_length++] = input[i];
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

    if (strchr(input, '\n') == NULL && !feof(stdin)) {
        int ch;

        while ((ch = getchar()) != '\n' && ch != EOF) {
        }

        return EXIT_FAILURE;
    }

    input[strcspn(input, "\n")] = '\0';

    if (fgets(character_line, sizeof(character_line), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    if (character_line[0] == '\n' || character_line[0] == '\0') {
        return EXIT_FAILURE;
    }

    if (character_line[1] != '\n' && character_line[1] != '\0') {
        return EXIT_FAILURE;
    }

    result = remove_character(input, character_line[0]);
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