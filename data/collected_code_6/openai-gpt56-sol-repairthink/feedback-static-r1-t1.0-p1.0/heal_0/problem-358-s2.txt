#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int modulo_lists(const int *dividends, const int *divisors,
                        int *results, size_t length)
{
    if (dividends == NULL || divisors == NULL || results == NULL) {
        return EINVAL;
    }

    for (size_t i = 0; i < length; ++i) {
        if (divisors[i] == 0) {
            return EDOM;
        }

        if (dividends[i] == INT_MIN && divisors[i] == -1) {
            results[i] = 0;
        } else {
            results[i] = dividends[i] % divisors[i];
        }
    }

    return 0;
}

int main(void)
{
    const int dividends[] = {17, 25, -14, INT_MIN};
    const int divisors[] = {5, 7, 4, -1};
    const size_t length = sizeof(dividends) / sizeof(dividends[0]);
    int *results = calloc(length, sizeof(*results));

    if (results == NULL) {
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    const int status = modulo_lists(dividends, divisors, results, length);

    if (status != 0) {
        fprintf(stderr, "Modulo operation failed: %d\n", status);
        free(results);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < length; ++i) {
        printf("%d%s", results[i], i + 1U < length ? " " : "\n");
    }

    free(results);
    return EXIT_SUCCESS;
}