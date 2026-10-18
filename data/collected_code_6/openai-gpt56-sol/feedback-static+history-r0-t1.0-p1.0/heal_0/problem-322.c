#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

static int find_min_indices(const int *values, size_t length,
                            size_t **indices, size_t *count)
{
    size_t i;
    size_t found;
    int minimum;
    size_t *result;

    if (values == NULL || length == 0U || indices == NULL || count == NULL) {
        return -1;
    }

    *indices = NULL;
    *count = 0U;
    minimum = values[0];
    found = 1U;

    for (i = 1U; i < length; ++i) {
        if (values[i] < minimum) {
            minimum = values[i];
            found = 1U;
        } else if (values[i] == minimum) {
            ++found;
        }
    }

    if (found > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(found * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    found = 0U;
    for (i = 0U; i < length; ++i) {
        if (values[i] == minimum) {
            result[found++] = i;
        }
    }

    *indices = result;
    *count = found;
    return 0;
}

int main(void)
{
    const int values[] = {4, 2, 7, 2, -1, 5, -1};
    const size_t length = sizeof(values) / sizeof(values[0]);
    size_t *indices = NULL;
    size_t count = 0U;
    size_t i;

    if (find_min_indices(values, length, &indices, &count) != 0) {
        fputs("Unable to find minimum indices.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < count; ++i) {
        printf("%zu%s", indices[i], i + 1U == count ? "\n" : " ");
    }

    free(indices);
    return EXIT_SUCCESS;
}