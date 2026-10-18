#include <errno.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int divide_lists(const double *numerators,
                        const double *denominators,
                        double *results,
                        size_t length)
{
    size_t i;

    if (numerators == NULL || denominators == NULL || results == NULL) {
        return EINVAL;
    }

    for (i = 0U; i < length; ++i) {
        if (denominators[i] == 0.0) {
            return EDOM;
        }

        results[i] = numerators[i] / denominators[i];

        if (!isfinite(results[i])) {
            return ERANGE;
        }
    }

    return 0;
}

int main(void)
{
    const double numerators[] = {10.0, 20.0, 30.0, 40.0};
    const double denominators[] = {2.0, 4.0, 5.0, 8.0};
    const size_t length = sizeof(numerators) / sizeof(numerators[0]);
    double *results;
    size_t i;
    int status;

    if (length != sizeof(denominators) / sizeof(denominators[0])) {
        return EXIT_FAILURE;
    }

    if (length > SIZE_MAX / sizeof(*results)) {
        return EXIT_FAILURE;
    }

    results = malloc(length * sizeof(*results));
    if (results == NULL) {
        return EXIT_FAILURE;
    }

    status = divide_lists(numerators, denominators, results, length);
    if (status != 0) {
        free(results);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < length; ++i) {
        if (printf("%.6g\n", results[i]) < 0) {
            free(results);
            return EXIT_FAILURE;
        }
    }

    free(results);
    return EXIT_SUCCESS;
}