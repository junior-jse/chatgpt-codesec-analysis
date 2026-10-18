#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void free_string_list(char **list, size_t count)
{
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(list[i]);
    }

    free(list);
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

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, source, length + 1U);
    return copy;
}

static int remove_duplicate_words(const char *const input[],
                                  size_t input_count,
                                  char ***output,
                                  size_t *output_count)
{
    char **unique;
    size_t unique_count = 0U;

    if (output == NULL || output_count == NULL ||
        (input == NULL && input_count != 0U)) {
        return -1;
    }

    *output = NULL;
    *output_count = 0U;

    if (input_count == 0U) {
        return 0;
    }

    if (input_count > SIZE_MAX / sizeof(*unique)) {
        return -1;
    }

    unique = calloc(input_count, sizeof(*unique));
    if (unique == NULL) {
        return -1;
    }

    for (size_t i = 0U; i < input_count; ++i) {
        int found = 0;

        if (input[i] == NULL) {
            free_string_list(unique, unique_count);
            return -1;
        }

        for (size_t j = 0U; j < unique_count; ++j) {
            if (strcmp(input[i], unique[j]) == 0) {
                found = 1;
                break;
            }
        }

        if (found == 0) {
            unique[unique_count] = duplicate_string(input[i]);
            if (unique[unique_count] == NULL) {
                free_string_list(unique, unique_count);
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
        "apple",
        "banana",
        "apple",
        "orange",
        "banana",
        "grape"
    };
    const size_t word_count = sizeof(words) / sizeof(words[0]);
    char **unique_words = NULL;
    size_t unique_count = 0U;

    if (remove_duplicate_words(words, word_count,
                               &unique_words, &unique_count) != 0) {
        fputs("Failed to remove duplicate words.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < unique_count; ++i) {
        if (puts(unique_words[i]) == EOF) {
            free_string_list(unique_words, unique_count);
            return EXIT_FAILURE;
        }
    }

    free_string_list(unique_words, unique_count);
    return EXIT_SUCCESS;
}