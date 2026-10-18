#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void free_strings(char **strings, size_t count)
{
    if (strings == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(strings[i]);
    }

    free(strings);
}

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    length = strlen(source);
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, source, length + 1);
    return copy;
}

static int remove_duplicate_words(const char *const input[],
                                  size_t input_count,
                                  char ***output,
                                  size_t *output_count)
{
    char **unique;
    size_t unique_count = 0;

    if (output == NULL || output_count == NULL ||
        (input == NULL && input_count != 0) ||
        input_count > SIZE_MAX / sizeof(*unique)) {
        return -1;
    }

    *output = NULL;
    *output_count = 0;

    if (input_count == 0) {
        return 0;
    }

    unique = calloc(input_count, sizeof(*unique));
    if (unique == NULL) {
        return -1;
    }

    for (size_t i = 0; i < input_count; ++i) {
        int duplicate = 0;

        if (input[i] == NULL) {
            free_strings(unique, unique_count);
            return -1;
        }

        for (size_t j = 0; j < unique_count; ++j) {
            if (strcmp(input[i], unique[j]) == 0) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            unique[unique_count] = duplicate_string(input[i]);
            if (unique[unique_count] == NULL) {
                free_strings(unique, unique_count);
                return -1;
            }
            ++unique_count;
        }
    }

    *output = unique;
    *output_count = unique_count;
    return 0;
}

int main(void)
{
    const char *words[] = {
        "apple", "banana", "apple", "orange",
        "banana", "grape", "orange"
    };
    const size_t word_count = sizeof(words) / sizeof(words[0]);
    char **unique_words = NULL;
    size_t unique_count = 0;

    if (remove_duplicate_words(words, word_count,
                               &unique_words, &unique_count) != 0) {
        fputs("Failed to remove duplicate words.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < unique_count; ++i) {
        if (puts(unique_words[i]) == EOF) {
            free_strings(unique_words, unique_count);
            return EXIT_FAILURE;
        }
    }

    free_strings(unique_words, unique_count);
    return EXIT_SUCCESS;
}