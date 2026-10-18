#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source);
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1U);
    return copy;
}

static void free_string_list(char **list, size_t count)
{
    size_t i;

    if (list == NULL) {
        return;
    }

    for (i = 0U; i < count; ++i) {
        free(list[i]);
    }

    free(list);
}

static int remove_duplicate_words(const char *const input[],
                                  size_t input_count,
                                  char ***output,
                                  size_t *output_count)
{
    char **unique;
    size_t unique_count = 0U;
    size_t i;

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

    for (i = 0U; i < input_count; ++i) {
        size_t j;
        int duplicate = 0;

        if (input[i] == NULL) {
            free_string_list(unique, unique_count);
            return -1;
        }

        for (j = 0U; j < unique_count; ++j) {
            if (strcmp(input[i], unique[j]) == 0) {
                duplicate = 1;
                break;
            }
        }

        if (duplicate == 0) {
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
        "apple", "banana", "apple", "orange", "banana", "grape"
    };
    char **unique_words = NULL;
    size_t unique_count = 0U;
    size_t i;
    int result;

    result = remove_duplicate_words(
        words,
        sizeof(words) / sizeof(words[0]),
        &unique_words,
        &unique_count
    );

    if (result != 0) {
        fputs("Failed to remove duplicate words.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < unique_count; ++i) {
        if (puts(unique_words[i]) == EOF) {
            free_string_list(unique_words, unique_count);
            return EXIT_FAILURE;
        }
    }

    free_string_list(unique_words, unique_count);
    return EXIT_SUCCESS;
}