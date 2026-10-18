#include <stdio.h>
#include <limits.h>
#include <stdint.h>

int multiply(int a, int b) {
    if (a == 0 || b == 0) return 0;
    
    int result = 0;
    int multiplicand = a < 0 ? -a : a;
    int multiplier = b < 0 ? -b : b;
    int negative = (a < 0) ^ (b < 0);
    
    while (multiplier > 0) {
        if (multiplier & 1) {
            if (negative && result > INT_MAX - multiplicand) {
                return INT_MIN;
            }
            if (!negative && result > INT_MAX - multiplicand) {
                return INT_MAX;
            }
            result += multiplicand;
        }
        multiplicand <<= 1;
        multiplier >>= 1;
    }
    
    return negative ? -result : result;
}

int main(void) {
    printf("3 * 4 = %d\n", multiply(3, 4));
    printf("-3 * 4 = %d\n", multiply(-3, 4));
    printf("3 * -4 = %d\n", multiply(3, -4));
    printf("-3 * -4 = %d\n", multiply(-3, -4));
    printf("0 * 5 = %d\n", multiply(0, 5));
    printf("INT_MAX * 1 = %d\n", multiply(INT_MAX, 1));
    printf("INT_MIN * 1 = %d\n", multiply(INT_MIN, 1));
    
    return 0;
}