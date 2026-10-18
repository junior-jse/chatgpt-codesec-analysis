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

static int remove_matching_words(const char *const input[],
                                 size_t input_count,
                                 const char *pattern,
                                 char ***output,
                                 size_t *output_count)
{
    char **result;
    size_t retained = 0U;

    if ((input == NULL && input_count != 0U) ||
        pattern == NULL ||
        output == NULL ||
        output_count == NULL) {
        return -1;
    }

    *output = NULL;
    *output_count = 0U;

    if (input_count == 0U) {
        return 0;
    }

    if (input_count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = calloc(input_count, sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    for (size_t i = 0U; i < input_count; ++i) {
        if (input[i] == NULL) {
            free_string_list(result, retained);
            return -1;
        }

        if (strstr(input[i], pattern) == NULL) {
            result[retained] = duplicate_string(input[i]);
            if (result[retained] == NULL) {
                free_string_list(result, retained);
                return -1;
            }
            ++retained;
        }
    }

    if (retained == 0U) {
        free(result);
        result = NULL;
    } else {
        char **resized = realloc(result, retained * sizeof(*result));
        if (resized != NULL) {
            result = resized;
        }
    }

    *output = result;
    *output_count = retained;
    return 0;
}

int main(void)
{
    const char *words[] = {
        "apple",
        "banana",
        "grape",
        "orange",
        "pineapple",
        "melon"
    };
    const char *pattern = "apple";
    char **filtered = NULL;
    size_t filtered_count = 0U;
    size_t word_count = sizeof(words) / sizeof(words[0]);

    if (remove_matching_words(words,
                              word_count,
                              pattern,
                              &filtered,
                              &filtered_count) != 0) {
        fputs("Failed to filter the string list.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < filtered_count; ++i) {
        if (puts(filtered[i]) == EOF) {
            free_string_list(filtered, filtered_count);
            return EXIT_FAILURE;
        }
    }

    free_string_list(filtered, filtered_count);
    return EXIT_SUCCESS;
}