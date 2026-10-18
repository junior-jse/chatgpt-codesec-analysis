#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef int (*map_fn)(int64_t, size_t, int64_t *);

static int checked_power(int64_t base, size_t exponent, int64_t *result)
{
    int64_t value = 1;

    if (result == NULL) {
        return -1;
    }

    for (size_t i = 0; i < exponent; ++i) {
        if ((base > 0 && (value > INT64_MAX / base || value < INT64_MIN / base)) ||
            (base < -1 && (value > INT64_MIN / base || value < INT64_MAX / base)) ||
            (base == -1 && value == INT64_MIN)) {
            return -1;
        }
        value *= base;
    }

    *result = value;
    return 0;
}

static int map(const int64_t *input, int64_t *output, size_t length, map_fn function)
{
    if ((length > 0 && (input == NULL || output == NULL)) || function == NULL) {
        return -1;
    }

    for (size_t i = 0; i < length; ++i) {
        if (function(input[i], i, &output[i]) != 0) {
            return -1;
        }
    }

    return 0;
}

static int create_power_list(const int64_t *bases, size_t length, int64_t **result)
{
    int64_t *powers;

    if (result == NULL || (length > 0 && bases == NULL)) {
        return -1;
    }

    *result = NULL;

    if (length == 0) {
        return 0;
    }

    if (length > SIZE_MAX / sizeof(*powers)) {
        return -1;
    }

    powers = malloc(length * sizeof(*powers));
    if (powers == NULL) {
        return -1;
    }

    if (map(bases, powers, length, checked_power) != 0) {
        free(powers);
        return -1;
    }

    *result = powers;
    return 0;
}

int main(void)
{
    int64_t bases[] = {2, 3, 4, 5, 6};
    size_t length = sizeof(bases) / sizeof(bases[0]);
    int64_t *powers = NULL;

    if (create_power_list(bases, length, &powers) != 0) {
        fputs("Failed to create power list.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < length; ++i) {
        if (printf("%lld%s", (long long)powers[i], i + 1 == length ? "\n" : " ") < 0) {
            free(powers);
            return EXIT_FAILURE;
        }
    }

    free(powers);
    return EXIT_SUCCESS;
}