#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    long long first;
    long long second;
} Tuple;

static bool multiply_checked(long long a, long long b, long long *result)
{
    if (result == NULL) {
        return false;
    }

    if (a > 0) {
        if ((b > 0 && a > LLONG_MAX / b) ||
            (b < 0 && b < LLONG_MIN / a)) {
            return false;
        }
    } else if (a < 0) {
        if ((b > 0 && a < LLONG_MIN / b) ||
            (b < 0 && a < LLONG_MAX / b)) {
            return false;
        }
    }

    *result = a * b;
    return true;
}

static bool find_max_product(const Tuple *tuples, size_t count, long long *maximum)
{
    if (tuples == NULL || maximum == NULL || count == 0U) {
        return false;
    }

    bool found = false;
    long long max_product = 0;

    for (size_t i = 0U; i < count; ++i) {
        long long product;

        if (!multiply_checked(tuples[i].first, tuples[i].second, &product)) {
            return false;
        }

        if (!found || product > max_product) {
            max_product = product;
            found = true;
        }
    }

    *maximum = max_product;
    return true;
}

int main(void)
{
    const Tuple tuples[] = {
        {2, 3},
        {-4, 5},
        {-6, -7},
        {8, 4}
    };
    const size_t count = sizeof(tuples) / sizeof(tuples[0]);
    long long maximum;

    if (!find_max_product(tuples, count, &maximum)) {
        fputs("Unable to calculate the maximum product.\n", stderr);
        return 1;
    }

    if (printf("%lld\n", maximum) < 0) {
        return 1;
    }

    return 0;
}