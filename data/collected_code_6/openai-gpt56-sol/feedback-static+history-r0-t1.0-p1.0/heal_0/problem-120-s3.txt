#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long long first;
    long long second;
} Tuple;

static int checked_multiply(long long a, long long b, long long *result)
{
    if (result == NULL) {
        return 0;
    }

    if (a > 0) {
        if ((b > 0 && a > LLONG_MAX / b) ||
            (b < 0 && b < LLONG_MIN / a)) {
            return 0;
        }
    } else if (a < 0) {
        if ((b > 0 && a < LLONG_MIN / b) ||
            (b < 0 && a < LLONG_MAX / b)) {
            return 0;
        }
    }

    *result = a * b;
    return 1;
}

static int find_maximum_product(const Tuple *tuples, size_t count,
                                long long *maximum)
{
    long long product;
    size_t i;

    if (tuples == NULL || maximum == NULL || count == 0) {
        return 0;
    }

    if (!checked_multiply(tuples[0].first, tuples[0].second, maximum)) {
        return 0;
    }

    for (i = 1; i < count; ++i) {
        if (!checked_multiply(tuples[i].first, tuples[i].second, &product)) {
            return 0;
        }

        if (product > *maximum) {
            *maximum = product;
        }
    }

    return 1;
}

int main(void)
{
    Tuple *tuples = NULL;
    size_t count;
    size_t i;
    long long maximum;
    int status = EXIT_FAILURE;

    if (scanf("%zu", &count) != 1 || count == 0 ||
        count > SIZE_MAX / sizeof(*tuples)) {
        fprintf(stderr, "Invalid tuple count.\n");
        goto cleanup;
    }

    tuples = malloc(count * sizeof(*tuples));
    if (tuples == NULL) {
        fprintf(stderr, "Memory allocation failed: errno=%d\n", errno);
        goto cleanup;
    }

    for (i = 0; i < count; ++i) {
        if (scanf("%lld %lld", &tuples[i].first, &tuples[i].second) != 2) {
            fprintf(stderr, "Invalid tuple data.\n");
            goto cleanup;
        }
    }

    if (!find_maximum_product(tuples, count, &maximum)) {
        fprintf(stderr, "Unable to compute the maximum product.\n");
        goto cleanup;
    }

    if (printf("%lld\n", maximum) < 0) {
        fprintf(stderr, "Output failed.\n");
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    free(tuples);
    return status;
}