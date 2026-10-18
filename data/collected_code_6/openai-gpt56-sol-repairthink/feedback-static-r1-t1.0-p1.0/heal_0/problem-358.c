#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

int modulo_lists(const int dividends[], const int divisors[], int results[],
                 size_t length)
{
    if (dividends == NULL || divisors == NULL || results == NULL) {
        return -1;
    }

    for (size_t i = 0; i < length; ++i) {
        if (divisors[i] == 0) {
            return -2;
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
    const int dividends[] = {10, 20, -15, INT_MIN, 7};
    const int divisors[] = {3, 6, 4, -1, 2};
    int results[sizeof(dividends) / sizeof(dividends[0])] = {0};
    const size_t length = sizeof(dividends) / sizeof(dividends[0]);

    int status = modulo_lists(dividends, divisors, results, length);
    if (status != 0) {
        errno = EDOM;
        perror("modulo_lists");
        return 1;
    }

    for (size_t i = 0; i < length; ++i) {
        if (printf("%d%s", results[i], i + 1U == length ? "\n" : " ") < 0) {
            return 1;
        }
    }

    return 0;
}