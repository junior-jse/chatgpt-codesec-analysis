#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

static uint64_t magnitude(int64_t value)
{
    uint64_t bits = (uint64_t)value;
    return value < 0 ? UINT64_C(0) - bits : bits;
}

static int multiply(int a, int b, int64_t *result)
{
    if (result == NULL) {
        return 0;
    }

    uint64_t multiplicand = magnitude(a);
    uint64_t multiplier = magnitude(b);
    uint64_t product = 0;

    while (multiplier != 0U) {
        if ((multiplier & UINT64_C(1)) != 0U) {
            product += multiplicand;
        }

        multiplier >>= 1U;

        if (multiplier != 0U) {
            multiplicand <<= 1U;
        }
    }

    if ((a < 0) != (b < 0)) {
        *result = -(int64_t)product;
    } else {
        *result = (int64_t)product;
    }

    return 1;
}

int main(void)
{
    int a;
    int b;
    int64_t result;

    if (scanf("%d %d", &a, &b) != 2) {
        fputs("Invalid input\n", stderr);
        return 1;
    }

    if (!multiply(a, b, &result)) {
        fputs("Multiplication failed\n", stderr);
        return 1;
    }

    if (printf("%" PRId64 "\n", result) < 0) {
        return 1;
    }

    return 0;
}