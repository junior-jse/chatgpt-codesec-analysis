#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static int compare_uint64(const void *lhs, const void *rhs)
{
    const uint64_t a = *(const uint64_t *)lhs;
    const uint64_t b = *(const uint64_t *)rhs;

    return (a > b) - (a < b);
}

static int divides(uint64_t a, uint64_t b)
{
    return (a == 0U) ? (b == 0U) : (b % a == 0U);
}

static int largest_divisible_subset(const uint64_t *values, size_t count,
                                    uint64_t **subset, size_t *subset_count)
{
    uint64_t *sorted = NULL;
    uint64_t *result = NULL;
    size_t *lengths = NULL;
    size_t *previous = NULL;
    size_t best_index = 0U;
    size_t best_length = 0U;

    if (subset == NULL || subset_count == NULL ||
        (values == NULL && count != 0U)) {
        return -1;
    }

    *subset = NULL;
    *subset_count = 0U;

    if (count == 0U) {
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

    for (size_t i = 0U; i < count; ++i) {
        sorted[i] = values[i];
    }

    qsort(sorted, count, sizeof(*sorted), compare_uint64);

    for (size_t i = 0U; i < count; ++i) {
        lengths[i] = 1U;
        previous[i] = SIZE_MAX;

        for (size_t j = 0U; j < i; ++j) {
            if (divides(sorted[j], sorted[i]) &&
                lengths[j] + 1U > lengths[i]) {
                lengths[i] = lengths[j] + 1U;
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

    {
        size_t index = best_index;

        for (size_t position = best_length; position > 0U; --position) {
            result[position - 1U] = sorted[index];
            if (previous[index] == SIZE_MAX) {
                break;
            }
            index = previous[index];
        }
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
    size_t count = 0U;
    uint64_t *values = NULL;
    uint64_t *subset = NULL;
    size_t subset_count = 0U;

    if (scanf("%zu", &count) != 1) {
        return EXIT_FAILURE;
    }

    if (count > SIZE_MAX / sizeof(*values)) {
        return EXIT_FAILURE;
    }

    if (count != 0U) {
        values = malloc(count * sizeof(*values));
        if (values == NULL) {
            return EXIT_FAILURE;
        }

        for (size_t i = 0U; i < count; ++i) {
            if (scanf("%" SCNu64, &values[i]) != 1) {
                free(values);
                return EXIT_FAILURE;
            }
        }
    }

    if (largest_divisible_subset(values, count, &subset, &subset_count) != 0) {
        free(values);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < subset_count; ++i) {
        if (i != 0U && putchar(' ') == EOF) {
            free(values);
            free(subset);
            return EXIT_FAILURE;
        }

        if (printf("%" PRIu64, subset[i]) < 0) {
            free(values);
            free(subset);
            return EXIT_FAILURE;
        }
    }

    if (putchar('\n') == EOF) {
        free(values);
        free(subset);
        return EXIT_FAILURE;
    }

    free(values);
    free(subset);
    return EXIT_SUCCESS;
}