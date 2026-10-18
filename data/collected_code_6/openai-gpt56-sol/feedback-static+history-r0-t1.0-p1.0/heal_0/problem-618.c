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
    if (numerators == NULL || denominators == NULL || results == NULL) {
        return EINVAL;
    }

    for (size_t i = 0U; i < length; ++i) {
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
    double *results = calloc(length, sizeof(*results));

    if (results == NULL) {
        fputs("Memory allocation failed.\n", stderr);
        return EXIT_FAILURE;
    }

    int status = divide_lists(numerators, denominators, results, length);

    if (status != 0) {
        fprintf(stderr, "Division failed with error code %d.\n", status);
        free(results);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < length; ++i) {
        if (printf("%.6g\n", results[i]) < 0) {
            fputs("Output failed.\n", stderr);
            free(results);
            return EXIT_FAILURE;
        }
    }

    free(results);
    return EXIT_SUCCESS;
}