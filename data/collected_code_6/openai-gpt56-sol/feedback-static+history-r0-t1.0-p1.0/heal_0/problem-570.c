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

static int remove_matching_strings(const char *const input[],
                                   size_t input_count,
                                   const char *pattern,
                                   char ***output,
                                   size_t *output_count)
{
    char **result;
    size_t kept = 0;

    if (input == NULL || pattern == NULL || output == NULL ||
        output_count == NULL || pattern[0] == '\0') {
        return -1;
    }

    *output = NULL;
    *output_count = 0;

    if (input_count == 0) {
        return 0;
    }

    if (input_count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = calloc(input_count, sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    for (size_t i = 0; i < input_count; ++i) {
        if (input[i] == NULL) {
            free_string_list(result, kept);
            return -1;
        }

        if (strstr(input[i], pattern) == NULL) {
            result[kept] = duplicate_string(input[i]);
            if (result[kept] == NULL) {
                free_string_list(result, kept);
                return -1;
            }
            ++kept;
        }
    }

    if (kept == 0) {
        free(result);
        result = NULL;
    } else {
        char **resized = realloc(result, kept * sizeof(*result));
        if (resized != NULL) {
            result = resized;
        }
    }

    *output = result;
    *output_count = kept;
    return 0;
}

int main(int argc, char *argv[])
{
    char **filtered = NULL;
    size_t filtered_count = 0;
    size_t input_count;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <character-or-string> <word> [word ...]\n",
                argc > 0 ? argv[0] : "program");
        return EXIT_FAILURE;
    }

    input_count = (size_t)argc - 2U;

    if (remove_matching_strings((const char *const *)&argv[2],
                                input_count,
                                argv[1],
                                &filtered,
                                &filtered_count) != 0) {
        fputs("Failed to filter the string list.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < filtered_count; ++i) {
        if (puts(filtered[i]) == EOF) {
            free_string_list(filtered, filtered_count);
            return EXIT_FAILURE;
        }
    }

    free_string_list(filtered, filtered_count);
    return EXIT_SUCCESS;
}