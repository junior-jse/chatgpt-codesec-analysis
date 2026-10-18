#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long long first;
    long long second;
} Tuple;

static int compare_long_long(const void *lhs, const void *rhs)
{
    const long long a = *(const long long *)lhs;
    const long long b = *(const long long *)rhs;

    return (a > b) - (a < b);
}

static unsigned long long absolute_difference(long long a, long long b)
{
    if (a >= b) {
        return (unsigned long long)a - (unsigned long long)b;
    }

    return (unsigned long long)b - (unsigned long long)a;
}

static int find_minimum_tuple_difference(
    const Tuple *tuples,
    size_t count,
    unsigned long long *minimum_difference)
{
    long long *values = NULL;
    unsigned long long minimum;
    size_t value_count;
    size_t i;

    if (tuples == NULL || minimum_difference == NULL || count == 0U) {
        return -1;
    }

    if (count > SIZE_MAX / 2U) {
        return -1;
    }

    value_count = count * 2U;

    if (value_count > SIZE_MAX / sizeof(*values)) {
        return -1;
    }

    values = malloc(value_count * sizeof(*values));
    if (values == NULL) {
        return -1;
    }

    for (i = 0U; i < count; ++i) {
        values[i * 2U] = tuples[i].first;
        values[i * 2U + 1U] = tuples[i].second;
    }

    qsort(values, value_count, sizeof(*values), compare_long_long);

    minimum = ULLONG_MAX;
    for (i = 1U; i < value_count; ++i) {
        const unsigned long long difference =
            absolute_difference(values[i], values[i - 1U]);

        if (difference < minimum) {
            minimum = difference;
        }
    }

    free(values);
    *minimum_difference = minimum;
    return 0;
}

int main(void)
{
    size_t count;
    Tuple *tuples = NULL;
    unsigned long long minimum_difference;
    size_t i;

    if (scanf("%zu", &count) != 1 || count == 0U) {
        fputs("Invalid tuple count\n", stderr);
        return EXIT_FAILURE;
    }

    if (count > SIZE_MAX / sizeof(*tuples)) {
        fputs("Tuple count is too large\n", stderr);
        return EXIT_FAILURE;
    }

    tuples = malloc(count * sizeof(*tuples));
    if (tuples == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    for (i = 0U; i < count; ++i) {
        if (scanf("%lld %lld", &tuples[i].first, &tuples[i].second) != 2) {
            fputs("Invalid tuple input\n", stderr);
            free(tuples);
            return EXIT_FAILURE;
        }
    }

    if (find_minimum_tuple_difference(
            tuples, count, &minimum_difference) != 0) {
        fputs("Unable to calculate the minimum difference\n", stderr);
        free(tuples);
        return EXIT_FAILURE;
    }

    if (printf("%llu\n", minimum_difference) < 0) {
        free(tuples);
        return EXIT_FAILURE;
    }

    free(tuples);
    return EXIT_SUCCESS;
}