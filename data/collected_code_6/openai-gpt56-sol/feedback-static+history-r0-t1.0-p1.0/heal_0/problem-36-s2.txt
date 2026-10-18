#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

static int nth_fractional_digit(uint64_t numerator, uint64_t denominator,
                                uint64_t position, unsigned int *digit)
{
    uint64_t remainder;

    if (denominator == 0 || position == 0 || digit == NULL) {
        return 0;
    }

    remainder = numerator % denominator;

    for (uint64_t i = 1; i <= position; ++i) {
        uint64_t quotient_digit = 0;

        for (unsigned int j = 0; j < 10; ++j) {
            if (remainder >= denominator) {
                remainder -= denominator;
                ++quotient_digit;
            }

            if (j != 9) {
                if (remainder > UINT64_MAX - remainder) {
                    return 0;
                }
                remainder += remainder;
            }
        }

        *digit = (unsigned int)quotient_digit;
    }

    return 1;
}

int main(void)
{
    uint64_t numerator;
    uint64_t denominator;
    uint64_t position;
    unsigned int digit;

    if (scanf("%" SCNu64 " %" SCNu64 " %" SCNu64,
              &numerator, &denominator, &position) != 3) {
        fputs("Invalid input\n", stderr);
        return 1;
    }

    if (!nth_fractional_digit(numerator, denominator, position, &digit)) {
        fputs("Unable to calculate digit\n", stderr);
        return 1;
    }

    printf("%u\n", digit);
    return 0;
}