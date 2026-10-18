#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t longest_repeating_subsequence(const char *string)
{
    size_t length;
    size_t *previous;
    size_t *current;
    size_t result = 0;

    if (string == NULL) {
        return 0;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    length = strlen(string);

    if (length == 0 || length == SIZE_MAX) {
        return 0;
    }

    previous = calloc(length + 1, sizeof(*previous));
    current = calloc(length + 1, sizeof(*current));

    if (previous == NULL || current == NULL) {
        free(previous);
        free(current);
        return 0;
    }

    for (size_t i = 1; i <= length; ++i) {
        current[0] = 0;

        for (size_t j = 1; j <= length; ++j) {
            if (string[i - 1] == string[j - 1] && i != j) {
                current[j] = previous[j - 1] + 1;
            } else {
                current[j] = previous[j] > current[j - 1]
                                 ? previous[j]
                                 : current[j - 1];
            }
        }

        size_t *temporary = previous;
        previous = current;
        current = temporary;
    }

    result = previous[length];
    free(previous);
    free(current);

    return result;
}

int main(void)
{
    char *input = NULL;
    size_t capacity = 0;
    size_t length = 0;
    int character;

    /* Possible weaknesses found:
     * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((character = getchar()) != '\n' && character != EOF) {
        if (length == capacity) {
            size_t new_capacity = capacity == 0 ? 64 : capacity * 2;

            if (new_capacity <= capacity ||
                new_capacity > SIZE_MAX / sizeof(*input)) {
                free(input);
                return EXIT_FAILURE;
            }

            char *resized = realloc(input, new_capacity * sizeof(*input));
            if (resized == NULL) {
                free(input);
                return EXIT_FAILURE;
            }

            input = resized;
            capacity = new_capacity;
        }

        input[length++] = (char)character;
    }

    if (character == EOF && ferror(stdin)) {
        free(input);
        return EXIT_FAILURE;
    }

    if (length == capacity) {
        size_t new_capacity = capacity == 0 ? 1 : capacity + 1;

        if (new_capacity <= capacity ||
            new_capacity > SIZE_MAX / sizeof(*input)) {
            free(input);
            return EXIT_FAILURE;
        }

        char *resized = realloc(input, new_capacity * sizeof(*input));
        if (resized == NULL) {
            free(input);
            return EXIT_FAILURE;
        }

        input = resized;
    }

    input[length] = '\0';
    printf("%zu\n", longest_repeating_subsequence(input));
    free(input);

    return EXIT_SUCCESS;
}