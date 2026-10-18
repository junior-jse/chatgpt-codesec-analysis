#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef int (*MapFunction)(int value, size_t index, int *result);

static int checked_power(int base, size_t exponent, int *result)
{
    int value = 1;

    if (result == NULL) {
        return -1;
    }

    for (size_t i = 0; i < exponent; ++i) {
        if (base > 0 && value > INT_MAX / base) {
            return -1;
        }
        if (base < 0 && value > 0 && base < INT_MIN / value) {
            return -1;
        }
        if (base < 0 && value < 0 && value < INT_MAX / base) {
            return -1;
        }
        value *= base;
    }

    *result = value;
    return 0;
}

static int power_by_index(int value, size_t index, int *result)
{
    return checked_power(value, index, result);
}

static int *map(const int *values, size_t count, MapFunction function)
{
    int *mapped;

    if ((values == NULL && count != 0) || function == NULL) {
        return NULL;
    }

    if (count == 0) {
        return calloc(1, sizeof(*mapped));
    }

    if (count > SIZE_MAX / sizeof(*mapped)) {
        return NULL;
    }

    mapped = malloc(count * sizeof(*mapped));
    if (mapped == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        if (function(values[i], i, &mapped[i]) != 0) {
            free(mapped);
            return NULL;
        }
    }

    return mapped;
}

int main(void)
{
    int bases[] = {2, 3, 4, 5, 6};
    size_t count = sizeof(bases) / sizeof(bases[0]);
    int *powers = map(bases, count, power_by_index);

    if (powers == NULL) {
        fputs("Failed to create the power list.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; ++i) {
        if (printf("%d%s", powers[i], i + 1 == count ? "\n" : " ") < 0) {
            free(powers);
            return EXIT_FAILURE;
        }
    }

    free(powers);
    return EXIT_SUCCESS;
}