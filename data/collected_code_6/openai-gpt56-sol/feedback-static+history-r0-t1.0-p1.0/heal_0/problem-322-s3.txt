#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

static int find_min_indices(const int *values, size_t count,
                            size_t **indices, size_t *index_count)
{
    size_t i;
    int minimum;
    size_t *result;

    if (values == NULL || indices == NULL || index_count == NULL || count == 0) {
        return -1;
    }

    *indices = NULL;
    *index_count = 0;

    if (count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(count * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    minimum = values[0];
    result[0] = 0;
    *index_count = 1;

    for (i = 1; i < count; ++i) {
        if (values[i] < minimum) {
            minimum = values[i];
            result[0] = i;
            *index_count = 1;
        } else if (values[i] == minimum) {
            result[*index_count] = i;
            ++(*index_count);
        }
    }

    *indices = result;
    return 0;
}

int main(void)
{
    const int values[] = {4, 2, 7, 2, 9, 2, 5};
    const size_t count = sizeof(values) / sizeof(values[0]);
    size_t *indices = NULL;
    size_t index_count = 0;
    size_t i;

    if (find_min_indices(values, count, &indices, &index_count) != 0) {
        fputs("Failed to find minimum indices.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < index_count; ++i) {
        printf("%zu%s", indices[i], i + 1 < index_count ? " " : "\n");
    }

    free(indices);
    return EXIT_SUCCESS;
}