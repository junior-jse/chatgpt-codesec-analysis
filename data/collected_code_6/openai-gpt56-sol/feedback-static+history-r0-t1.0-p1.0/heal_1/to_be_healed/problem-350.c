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

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
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
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[4096];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
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