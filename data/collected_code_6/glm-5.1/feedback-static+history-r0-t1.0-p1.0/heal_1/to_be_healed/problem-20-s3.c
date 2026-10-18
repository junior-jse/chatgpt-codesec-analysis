#include <stdio.h>
#include <stdbool.h>
 /* Possible weaknesses found:
  *  'ULLONG_MAX' is defined in header '<limits.h>'; this is probably fixable by adding '#include <limits.h>'
  */

bool is_woodall(unsigned long long n) {
    if (n == 0) {
        return false;
    }
    unsigned long long i = 1;
    unsigned long long power = 2;
    unsigned long long result = power * i - 1;
    while (result < n) {
        i++;
        /* Possible weaknesses found:
         *  use of undeclared identifier 'ULLONG_MAX'
         *  each undeclared identifier is reported only once for each function it appears in
         *  'ULLONG_MAX' undeclared (first use in this function)
         */
        if (power > (ULLONG_MAX / 2)) {
            return false;
        }
        power *= 2;
        /* Possible weaknesses found:
         *  use of undeclared identifier 'ULLONG_MAX'
         */
        if (i > (ULLONG_MAX / power)) {
            return false;
        }
        result = power * i - 1;
    }
    return result == n;
}

int main(void) {
    unsigned long long num;
    if (scanf("%llu", &num) != 1) {
        return 1;
    }
    if (is_woodall(num)) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
    return 0;
}