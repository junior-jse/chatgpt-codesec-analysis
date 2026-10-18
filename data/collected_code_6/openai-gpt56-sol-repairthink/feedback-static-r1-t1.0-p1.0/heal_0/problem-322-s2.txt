#include <stdio.h>
#include <stdlib.h>

int find_min_indices(const int *values, size_t count, size_t **indices, size_t *index_count)
{
    size_t *result;
    size_t matches = 0;
    size_t i;
    int minimum;

    if (values == NULL || count == 0 || indices == NULL || index_count == NULL) {
        return -1;
    }

    *indices = NULL;
    *index_count = 0;
    minimum = values[0];

    for (i = 1; i < count; ++i) {
        if (values[i] < minimum) {
            minimum = values[i];
        }
    }

    for (i = 0; i < count; ++i) {
        if (values[i] == minimum) {
            ++matches;
        }
    }

    if (matches > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(matches * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    matches = 0;
    for (i = 0; i < count; ++i) {
        if (values[i] == minimum) {
            result[matches++] = i;
        }
    }

    *indices = result;
    *index_count = matches;
    return 0;
}

int main(void)
{
    int values[] = {4, 2, 7, 2, 9, 2};
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