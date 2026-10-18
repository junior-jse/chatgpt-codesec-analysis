#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int sign;
    unsigned long long significand;
    int exponent;
} FloatTuple;

int tuple_to_double(const FloatTuple *tuple, double *result)
{
    double value;

    if (tuple == NULL || result == NULL) {
        return -1;
    }

    if (tuple->sign != 1 && tuple->sign != -1) {
        return -1;
    }

    errno = 0;
    value = scalbn((double)tuple->significand, tuple->exponent);

    if (tuple->sign < 0) {
        value = -value;
    }

    if (!isfinite(value) || errno == ERANGE) {
        return -1;
    }

    *result = value;
    return 0;
}

int main(void)
{
    FloatTuple tuple = {1, 12345ULL, -2};
    double value;

    if (tuple_to_double(&tuple, &value) != 0) {
        fputs("Conversion failed\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%.17g\n", value) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}