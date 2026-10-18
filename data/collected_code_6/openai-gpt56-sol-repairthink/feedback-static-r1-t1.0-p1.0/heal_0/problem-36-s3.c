#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

static int nth_fractional_digit(uint64_t numerator, uint64_t denominator,
                                uint64_t n, unsigned int *digit)
{
    uint64_t remainder;

    if (denominator == 0 || n == 0 || digit == NULL) {
        return -1;
    }

    remainder = numerator % denominator;

    for (uint64_t position = 1; position <= n; ++position) {
        uint64_t quotient_digit = 0;

        for (unsigned int i = 0; i < 10; ++i) {
            if (remainder >= denominator) {
                remainder -= denominator;
                ++quotient_digit;
            }

            if (i != 9) {
                if (remainder > UINT64_MAX - remainder) {
                    return -1;
                }
                remainder += remainder;
            }
        }

        *digit = (unsigned int)quotient_digit;
    }

    return 0;
}

int main(void)
{
    uint64_t numerator;
    uint64_t denominator;
    uint64_t n;
    unsigned int digit;

    if (scanf("%" SCNu64 " %" SCNu64 " %" SCNu64,
              &numerator, &denominator, &n) != 3) {
        fputs("Invalid input\n", stderr);
        return 1;
    }

    if (nth_fractional_digit(numerator, denominator, n, &digit) != 0) {
        fputs("Unable to calculate digit\n", stderr);
        return 1;
    }

    printf("%u\n", digit);
    return 0;
}