#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static int combine_sorted_lists(const int *first, size_t first_size,
                                const int *second, size_t second_size,
                                int **result, size_t *result_size)
{
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    size_t total;
    int *combined;

    if (result == NULL || result_size == NULL ||
        (first == NULL && first_size != 0) ||
        (second == NULL && second_size != 0)) {
        return EINVAL;
    }

    *result = NULL;
    *result_size = 0;

    if (first_size > SIZE_MAX - second_size) {
        return EOVERFLOW;
    }

    total = first_size + second_size;

    if (total == 0) {
        return 0;
    }

    if (total > SIZE_MAX / sizeof(*combined)) {
        return EOVERFLOW;
    }

    combined = malloc(total * sizeof(*combined));
    if (combined == NULL) {
        return ENOMEM;
    }

    while (i < first_size && j < second_size) {
        if (first[i] <= second[j]) {
            combined[k++] = first[i++];
        } else {
            combined[k++] = second[j++];
        }
    }

    while (i < first_size) {
        combined[k++] = first[i++];
    }

    while (j < second_size) {
        combined[k++] = second[j++];
    }

    *result = combined;
    *result_size = total;
    return 0;
}

int main(void)
{
    const int first[] = {1, 3, 5, 7};
    const int second[] = {2, 4, 6, 8, 10};
    int *combined = NULL;
    size_t combined_size = 0;
    size_t i;
    int status;

    status = combine_sorted_lists(
        first, sizeof(first) / sizeof(first[0]),
        second, sizeof(second) / sizeof(second[0]),
        &combined, &combined_size
    );

    if (status != 0) {
        fprintf(stderr, "Failed to combine lists: %d\n", status);
        return EXIT_FAILURE;
    }

    for (i = 0; i < combined_size; ++i) {
        if (printf("%s%d", i == 0 ? "" : " ", combined[i]) < 0) {
            free(combined);
            return EXIT_FAILURE;
        }
    }

    if (putchar('\n') == EOF) {
        free(combined);
        return EXIT_FAILURE;
    }

    free(combined);
    return EXIT_SUCCESS;
}