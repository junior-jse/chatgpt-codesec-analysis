#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static int *combine_sorted_lists(const int *first, size_t first_size,
                                 const int *second, size_t second_size,
                                 size_t *combined_size)
{
    int *combined;
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    size_t total;

    if (combined_size == NULL ||
        (first_size > 0 && first == NULL) ||
        (second_size > 0 && second == NULL) ||
        first_size > SIZE_MAX - second_size) {
        return NULL;
    }

    total = first_size + second_size;
    *combined_size = total;

    if (total == 0) {
        return NULL;
    }

    if (total > SIZE_MAX / sizeof(*combined)) {
        *combined_size = 0;
        return NULL;
    }

    combined = malloc(total * sizeof(*combined));
    if (combined == NULL) {
        *combined_size = 0;
        return NULL;
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

    return combined;
}

int main(void)
{
    const int first[] = {1, 3, 5, 7, 9};
    const int second[] = {2, 4, 6, 8, 10};
    const size_t first_size = sizeof(first) / sizeof(first[0]);
    const size_t second_size = sizeof(second) / sizeof(second[0]);
    size_t combined_size = 0;
    int *combined = combine_sorted_lists(first, first_size, second,
                                         second_size, &combined_size);

    if (combined == NULL && combined_size != 0) {
        fprintf(stderr, "Failed to combine lists.\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < combined_size; ++i) {
        if (printf("%d%s", combined[i],
                   i + 1 < combined_size ? " " : "\n") < 0) {
            free(combined);
            return EXIT_FAILURE;
        }
    }

    free(combined);
    return EXIT_SUCCESS;
}