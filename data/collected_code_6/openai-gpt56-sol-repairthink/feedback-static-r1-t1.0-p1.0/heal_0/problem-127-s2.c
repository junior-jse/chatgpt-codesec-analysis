#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

static int multiply_int32(int32_t lhs, int32_t rhs, int32_t *result)
{
    int64_t multiplicand = lhs;
    uint64_t multiplier;
    int64_t product = 0;

    if (result == NULL) {
        return 0;
    }

    if (rhs < 0) {
        multiplicand = -multiplicand;
        multiplier = (uint64_t)(-(int64_t)rhs);
    } else {
        multiplier = (uint64_t)rhs;
    }

    while (multiplier != 0U) {
        if ((multiplier & UINT64_C(1)) != 0U) {
            product += multiplicand;
        }
        multiplier >>= 1U;
        if (multiplier != 0U) {
            multiplicand <<= 1U;
        }
    }

    if (product < INT32_MIN || product > INT32_MAX) {
        return 0;
    }

    *result = (int32_t)product;
    return 1;
}

int main(void)
{
    int32_t lhs;
    int32_t rhs;
    int32_t result;

    if (scanf("%" SCNd32 " %" SCNd32, &lhs, &rhs) != 2) {
        fputs("Invalid input\n", stderr);
        return 1;
    }

    if (!multiply_int32(lhs, rhs, &result)) {
        fputs("Multiplication overflow\n", stderr);
        return 1;
    }

    printf("%" PRId32 "\n", result);
    return 0;
}