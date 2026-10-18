#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef int (*MapFunction)(long long, size_t, long long *);

static int checked_power(long long base, size_t exponent, long long *result)
{
    long long value = 1;

    if (result == NULL) {
        return -1;
    }

    for (size_t i = 0; i < exponent; ++i) {
        if ((base > 0 && (value > LLONG_MAX / base || value < LLONG_MIN / base)) ||
            (base < -1 && (value > LLONG_MIN / base || value < LLONG_MAX / base)) ||
            (base == -1 && value == LLONG_MIN)) {
            return -1;
        }
        value *= base;
    }

    *result = value;
    return 0;
}

static long long *map(const long long *values, size_t count, MapFunction function)
{
    long long *result;

    if ((values == NULL && count != 0) || function == NULL) {
        return NULL;
    }

    if (count == 0) {
        return calloc(1, sizeof(*result));
    }

    if (count > SIZE_MAX / sizeof(*result)) {
        return NULL;
    }

    result = malloc(count * sizeof(*result));
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        if (function(values[i], i, &result[i]) != 0) {
            free(result);
            return NULL;
        }
    }

    return result;
}

int main(void)
{
    long long bases[] = {2, 3, 4, 5, 6};
    const size_t count = sizeof(bases) / sizeof(bases[0]);
    long long *powers = map(bases, count, checked_power);

    if (powers == NULL) {
        fputs("Failed to create the power list.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; ++i) {
        if (printf("%lld%s", powers[i], i + 1 == count ? "\n" : " ") < 0) {
            free(powers);
            return EXIT_FAILURE;
        }
    }

    free(powers);
    return EXIT_SUCCESS;
}