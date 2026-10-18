#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static int combine_sorted_lists(const int *first, size_t first_len,
                                const int *second, size_t second_len,
                                int **result, size_t *result_len)
{
    size_t total;
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    int *combined;

    if (result == NULL || result_len == NULL ||
        (first == NULL && first_len != 0) ||
        (second == NULL && second_len != 0)) {
        return -1;
    }

    *result = NULL;
    *result_len = 0;

    if (first_len > SIZE_MAX - second_len) {
        return -1;
    }

    total = first_len + second_len;

    if (total == 0) {
        return 0;
    }

    if (total > SIZE_MAX / sizeof(*combined)) {
        return -1;
    }

    combined = malloc(total * sizeof(*combined));
    if (combined == NULL) {
        return -1;
    }

    while (i < first_len && j < second_len) {
        if (first[i] <= second[j]) {
            combined[k++] = first[i++];
        } else {
            combined[k++] = second[j++];
        }
    }

    while (i < first_len) {
        combined[k++] = first[i++];
    }

    while (j < second_len) {
        combined[k++] = second[j++];
    }

    *result = combined;
    *result_len = total;
    return 0;
}

int main(void)
{
    const int first[] = {1, 3, 5, 7};
    const int second[] = {2, 4, 6, 8, 10};
    const size_t first_len = sizeof(first) / sizeof(first[0]);
    const size_t second_len = sizeof(second) / sizeof(second[0]);
    int *combined = NULL;
    size_t combined_len = 0;
    size_t i;

    if (combine_sorted_lists(first, first_len, second, second_len,
                             &combined, &combined_len) != 0) {
        fputs("Failed to combine sorted lists.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < combined_len; ++i) {
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