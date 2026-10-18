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

static int concatenate_adjacent(const char *const tuple[],
                                size_t count,
                                char ***result,
                                size_t *result_count)
{
    if (tuple == NULL || result == NULL || result_count == NULL) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    if (count < 2) {
        return 0;
    }

    char **output = calloc(count - 1, sizeof(*output));
    if (output == NULL) {
        return -1;
    }

    for (size_t i = 0; i < count - 1; ++i) {
        if (tuple[i] == NULL || tuple[i + 1] == NULL) {
            free_strings(output, i);
            return -1;
        }

        size_t first_length = strlen(tuple[i]);
        size_t second_length = strlen(tuple[i + 1]);

        if (first_length > SIZE_MAX - second_length - 1) {
            free_strings(output, i);
            return -1;
        }

        size_t combined_length = first_length + second_length;
        output[i] = malloc(combined_length + 1);

        if (output[i] == NULL) {
            free_strings(output, i);
            return -1;
        }

        memcpy(output[i], tuple[i], first_length);
        memcpy(output[i] + first_length, tuple[i + 1], second_length + 1);
    }

    *result = output;
    *result_count = count - 1;
    return 0;
}

int main(void)
{
    const char *tuple[] = {"abc", "def", "ghi", "jkl"};
    const size_t tuple_count = sizeof(tuple) / sizeof(tuple[0]);
    char **result = NULL;
    size_t result_count = 0;

    if (concatenate_adjacent(tuple, tuple_count, &result, &result_count) != 0) {
        fputs("Failed to concatenate adjacent elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < result_count; ++i) {
        if (printf("%s%s", result[i], i + 1 < result_count ? " " : "\n") < 0) {
            free_strings(result, result_count);
            return EXIT_FAILURE;
        }
    }

    free_strings(result, result_count);
    return EXIT_SUCCESS;
}