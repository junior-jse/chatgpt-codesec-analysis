#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

static int modulo_lists(const int *dividends,
                        const int *divisors,
                        int *results,
                        size_t length)
{
    size_t i;

    if ((length > 0U) &&
        ((dividends == NULL) || (divisors == NULL) || (results == NULL))) {
        return EINVAL;
    }

    for (i = 0U; i < length; ++i) {
        if (divisors[i] == 0) {
            return EDOM;
        }

        if ((dividends[i] == INT_MIN) && (divisors[i] == -1)) {
            results[i] = 0;
        } else {
            results[i] = dividends[i] % divisors[i];
        }
    }

    return 0;
}

int main(void)
{
    const int dividends[] = {17, 24, -15, INT_MIN, 42};
    const int divisors[] = {5, 7, 4, -1, 8};
    int results[sizeof(dividends) / sizeof(dividends[0])];
    const size_t length = sizeof(dividends) / sizeof(dividends[0]);
    int status;
    size_t i;

    if (length != (sizeof(divisors) / sizeof(divisors[0]))) {
        fputs("List lengths do not match.\n", stderr);
        return 1;
    }

    status = modulo_lists(dividends, divisors, results, length);
    if (status != 0) {
        if (status == EDOM) {
            fputs("Division by zero is not allowed.\n", stderr);
        } else {
            fputs("Invalid argument.\n", stderr);
        }
        return 1;
    }

    for (i = 0U; i < length; ++i) {
        if (printf("%d%s", results[i], (i + 1U == length) ? "\n" : " ") < 0) {
            return 1;
        }
    }

    return 0;
}