#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct {
    int64_t first;
    int64_t second;
} Tuple;

static bool multiply_checked(int64_t a, int64_t b, int64_t *result)
{
    if (result == NULL) {
        return false;
    }

    if ((a > 0 && ((b > 0 && a > INT64_MAX / b) ||
                   (b < 0 && b < INT64_MIN / a))) ||
        (a < 0 && ((b > 0 && a < INT64_MIN / b) ||
                   (b < 0 && a < INT64_MAX / b)))) {
        return false;
    }

    *result = a * b;
    return true;
}

static bool find_maximum_product(const Tuple *tuples, size_t count, int64_t *maximum)
{
    int64_t product = 0;
    bool found = false;

    if (tuples == NULL || maximum == NULL || count == 0U) {
        return false;
    }

    for (size_t i = 0U; i < count; ++i) {
        if (!multiply_checked(tuples[i].first, tuples[i].second, &product)) {
            return false;
        }

        if (!found || product > *maximum) {
            *maximum = product;
            found = true;
        }
    }

    return found;
}

int main(void)
{
    const Tuple tuples[] = {
        {2, 3},
        {-4, -5},
        {7, 6},
        {-8, 2}
    };
    const size_t count = sizeof(tuples) / sizeof(tuples[0]);
    int64_t maximum = 0;

    if (!find_maximum_product(tuples, count, &maximum)) {
        fputs("Unable to determine the maximum product.\n", stderr);
        return 1;
    }

    if (printf("Maximum product: %" PRId64 "\n", maximum) < 0) {
        return 1;
    }

    return 0;
}