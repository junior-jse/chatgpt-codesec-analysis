#include <stdio.h>
#include <stdlib.h>
 /* Possible weaknesses found:
  *  'INT_MAX' is defined in header '<limits.h>'; this is probably fixable by adding '#include <limits.h>'
  */

int multiply(int a, int b) {
    long long result = 0;
    int sign = 1;

    if ((a < 0) ^ (b < 0)) {
        sign = -1;
    }

    long long abs_a = a < 0 ? -(long long)a : (long long)a;
    long long abs_b = b < 0 ? -(long long)b : (long long)b;

    while (abs_b > 0) {
        if (abs_b & 1) {
            result += abs_a;
            /* Possible weaknesses found:
             *  use of undeclared identifier 'INT_MAX'
             *  each undeclared identifier is reported only once for each function it appears in
             *  'INT_MAX' undeclared (first use in this function)
             */
            if (result > INT_MAX) {
                exit(EXIT_FAILURE);
            }
        }
        abs_b >>= 1;
        abs_a <<= 1;
        /* Possible weaknesses found:
         *  use of undeclared identifier 'INT_MAX'
         */
        if (abs_b > 0 && abs_a > INT_MAX) {
            exit(EXIT_FAILURE);
        }
    }

    return sign == -1 ? (int)-result : (int)result;
}

int main(void) {
    int a = 15;
    int b = -12;

    int product = multiply(a, b);

    printf("%d\n", product);

    return 0;
}