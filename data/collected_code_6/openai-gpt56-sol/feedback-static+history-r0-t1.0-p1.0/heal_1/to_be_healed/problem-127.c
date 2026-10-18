#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

static uint64_t magnitude(int64_t value)
{
    uint64_t bits = (uint64_t)value;
    return value < 0 ? UINT64_C(0) - bits : bits;
}

static int multiply_int64(int64_t left, int64_t right, int64_t *result)
{
    uint64_t multiplicand;
    uint64_t multiplier;
    uint64_t product = 0;
    uint64_t limit;
    int negative;

    if (result == NULL) {
        return 0;
    }

    negative = (left < 0) != (right < 0);
    multiplicand = magnitude(left);
    multiplier = magnitude(right);
    limit = negative ? (UINT64_C(1) << 63) : (uint64_t)INT64_MAX;

    while (multiplier != 0) {
        if ((multiplier & UINT64_C(1)) != 0) {
            if (multiplicand > limit - product) {
                return 0;
            }
            product += multiplicand;
        }

        multiplier >>= 1;

        if (multiplier != 0) {
            if (multiplicand > limit - multiplicand) {
                return 0;
            }
            multiplicand += multiplicand;
        }
    }

    if (negative) {
        if (product == (UINT64_C(1) << 63)) {
            *result = INT64_MIN;
        } else {
            *result = -(int64_t)product;
        }
    } else {
        *result = (int64_t)product;
    }

    return 1;
}

int main(void)
{
    int64_t left;
    int64_t right;
    int64_t result;

    /* Possible weaknesses found:
     * Flawfinder scanf: The scanf() family's %s operation, without a limit specification, permits buffer overflows (CWE-120, CWE-20). Specify a limit to %s, or use a different input function. If the scanf format is influenceable by an attacker, it's exploitable. (risk 4, buffer)
     */
    if (scanf("%" SCNd64 " %" SCNd64, &left, &right) != 2) {
        fputs("Invalid input\n", stderr);
        return 1;
    }

    if (!multiply_int64(left, right, &result)) {
        fputs("Multiplication overflow\n", stderr);
        return 1;
    }

    if (printf("%" PRId64 "\n", result) < 0) {
        return 1;
    }

    return 0;
}