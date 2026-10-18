#include <stdio.h>
#include <stdlib.h>

static int find_common_indices(const int *a, const int *b, const int *c,
                               size_t length, size_t **indices,
                               size_t *index_count)
{
    size_t *result = NULL;
    size_t count = 0;

    if (a == NULL || b == NULL || c == NULL ||
        indices == NULL || index_count == NULL) {
        return -1;
    }

    *indices = NULL;
    *index_count = 0;

    if (length == 0) {
        return 0;
    }

    if (length > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(length * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    for (size_t i = 0; i < length; ++i) {
        if (a[i] == b[i] && b[i] == c[i]) {
            result[count++] = i;
        }
    }

    if (count == 0) {
        free(result);
        return 0;
    }

    *indices = result;
    *index_count = count;
    return 0;
}

int main(void)
{
    const int first[] = {1, 2, 3, 4, 5, 6};
    const int second[] = {1, 7, 3, 8, 5, 9};
    const int third[] = {1, 2, 3, 0, 5, 6};
    const size_t length = sizeof(first) / sizeof(first[0]);
    size_t *indices = NULL;
    size_t index_count = 0;

    if (find_common_indices(first, second, third, length,
                            &indices, &index_count) != 0) {
        fputs("Failed to find common indices.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < index_count; ++i) {
        if (printf("%zu%s", indices[i],
                   i + 1 == index_count ? "\n" : " ") < 0) {
            free(indices);
            return EXIT_FAILURE;
        }
    }

    if (index_count == 0 && puts("No common indices.") == EOF) {
        free(indices);
        return EXIT_FAILURE;
    }

    free(indices);
    return EXIT_SUCCESS;
}