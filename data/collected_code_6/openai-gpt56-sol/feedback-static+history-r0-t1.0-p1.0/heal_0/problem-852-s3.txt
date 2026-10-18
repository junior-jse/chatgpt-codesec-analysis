#include <stdio.h>
#include <stdlib.h>

static int remove_negative_numbers(const int *input, size_t count,
                                   int **output, size_t *output_count)
{
    int *result = NULL;
    size_t kept = 0;
    size_t index = 0;

    if (output == NULL || output_count == NULL || (input == NULL && count != 0)) {
        return -1;
    }

    *output = NULL;
    *output_count = 0;

    for (index = 0; index < count; ++index) {
        if (input[index] >= 0) {
            ++kept;
        }
    }

    if (kept == 0) {
        return 0;
    }

    if (kept > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(kept * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    kept = 0;
    for (index = 0; index < count; ++index) {
        if (input[index] >= 0) {
            result[kept++] = input[index];
        }
    }

    *output = result;
    *output_count = kept;
    return 0;
}

int main(void)
{
    const int numbers[] = {4, -2, 0, -7, 9, 3, -1};
    const size_t count = sizeof(numbers) / sizeof(numbers[0]);
    int *filtered = NULL;
    size_t filtered_count = 0;
    size_t index = 0;
    int status = EXIT_SUCCESS;

    if (remove_negative_numbers(numbers, count, &filtered, &filtered_count) != 0) {
        fputs("Failed to remove negative numbers.\n", stderr);
        return EXIT_FAILURE;
    }

    for (index = 0; index < filtered_count; ++index) {
        if (printf("%d%s", filtered[index],
                   index + 1U < filtered_count ? " " : "\n") < 0) {
            status = EXIT_FAILURE;
            break;
        }
    }

    free(filtered);
    return status;
}