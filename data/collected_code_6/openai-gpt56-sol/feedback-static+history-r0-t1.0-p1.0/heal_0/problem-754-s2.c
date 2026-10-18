#include <stdio.h>
#include <stdlib.h>

static int find_common_indices(const int *first, const int *second,
                               const int *third, size_t count,
                               size_t **indices, size_t *index_count)
{
    size_t *result;
    size_t found = 0;

    if (first == NULL || second == NULL || third == NULL ||
        indices == NULL || index_count == NULL) {
        return -1;
    }

    *indices = NULL;
    *index_count = 0;

    if (count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(count * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        if (first[i] == second[i] && second[i] == third[i]) {
            result[found++] = i;
        }
    }

    if (found == 0) {
        free(result);
        return 0;
    }

    *indices = result;
    *index_count = found;
    return 0;
}

int main(void)
{
    const int first[] = {4, 8, 15, 16, 23, 42};
    const int second[] = {4, 7, 15, 10, 23, 40};
    const int third[] = {4, 8, 15, 11, 23, 41};
    const size_t count = sizeof(first) / sizeof(first[0]);
    size_t *indices = NULL;
    size_t index_count = 0;

    if (find_common_indices(first, second, third, count,
                            &indices, &index_count) != 0) {
        fputs("Unable to find common indices.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < index_count; ++i) {
        if (printf("%zu\n", indices[i]) < 0) {
            free(indices);
            return EXIT_FAILURE;
        }
    }

    free(indices);
    return EXIT_SUCCESS;
}