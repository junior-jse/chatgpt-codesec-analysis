#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int compare_ints(const void *a, const void *b)
{
    const int left = *(const int *)a;
    const int right = *(const int *)b;

    return (left > right) - (left < right);
}

static int find_largest_divisible_subset(const int *values, size_t count,
                                         int **subset, size_t *subset_count)
{
    int *sorted = NULL;
    size_t *lengths = NULL;
    size_t *previous = NULL;
    int *result = NULL;
    size_t best_index = 0;
    size_t best_length = 0;

    if (subset == NULL || subset_count == NULL ||
        (count > 0 && values == NULL)) {
        return -1;
    }

    *subset = NULL;
    *subset_count = 0;

    if (count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*sorted) ||
        count > SIZE_MAX / sizeof(*lengths) ||
        count > SIZE_MAX / sizeof(*previous)) {
        return -1;
    }

    sorted = malloc(count * sizeof(*sorted));
    lengths = malloc(count * sizeof(*lengths));
    previous = malloc(count * sizeof(*previous));

    if (sorted == NULL || lengths == NULL || previous == NULL) {
        free(sorted);
        free(lengths);
        free(previous);
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        if (values[i] <= 0) {
            free(sorted);
            free(lengths);
            free(previous);
            return -1;
        }
        sorted[i] = values[i];
    }

    qsort(sorted, count, sizeof(*sorted), compare_ints);

    for (size_t i = 0; i < count; ++i) {
        lengths[i] = 1;
        previous[i] = SIZE_MAX;

        for (size_t j = 0; j < i; ++j) {
            if (sorted[i] % sorted[j] == 0 &&
                lengths[j] + 1 > lengths[i]) {
                lengths[i] = lengths[j] + 1;
                previous[i] = j;
            }
        }

        if (lengths[i] > best_length) {
            best_length = lengths[i];
            best_index = i;
        }
    }

    if (best_length > SIZE_MAX / sizeof(*result)) {
        free(sorted);
        free(lengths);
        free(previous);
        return -1;
    }

    result = malloc(best_length * sizeof(*result));
    if (result == NULL) {
        free(sorted);
        free(lengths);
        free(previous);
        return -1;
    }

    size_t index = best_index;
    for (size_t position = best_length; position > 0; --position) {
        result[position - 1] = sorted[index];
        index = previous[index];
    }

    free(sorted);
    free(lengths);
    free(previous);

    *subset = result;
    *subset_count = best_length;
    return 0;
}

int main(void)
{
    size_t count = 0;
    int *values = NULL;
    int *subset = NULL;
    size_t subset_count = 0;

    if (scanf("%zu", &count) != 1) {
        fprintf(stderr, "Invalid input.\n");
        return EXIT_FAILURE;
    }

    if (count > SIZE_MAX / sizeof(*values)) {
        fprintf(stderr, "Input is too large.\n");
        return EXIT_FAILURE;
    }

    if (count > 0) {
        values = malloc(count * sizeof(*values));
        if (values == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < count; ++i) {
        if (scanf("%d", &values[i]) != 1 || values[i] <= 0) {
            fprintf(stderr, "All values must be positive integers.\n");
            free(values);
            return EXIT_FAILURE;
        }
    }

    if (find_largest_divisible_subset(values, count, &subset,
                                      &subset_count) != 0) {
        fprintf(stderr, "Unable to find the subset.\n");
        free(values);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < subset_count; ++i) {
        if (i > 0) {
            putchar(' ');
        }
        printf("%d", subset[i]);
    }
    putchar('\n');

    free(subset);
    free(values);
    return EXIT_SUCCESS;
}