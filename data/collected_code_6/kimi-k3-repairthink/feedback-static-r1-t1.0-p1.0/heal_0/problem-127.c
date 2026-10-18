#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

int multiply(int a, int b, bool *overflow) {
    int result = 0;
    int negative = 0;
    unsigned int ua, ub;
    
    *overflow = false;
    
    if (a == 0 || b == 0) {
        return 0;
    }
    
    if (a < 0) {
        negative = !negative;
        if (a == INT_MIN) {
            ua = (unsigned int)INT_MAX + 1;
        } else {
            ua = (unsigned int)(-a);
        }
    } else {
        ua = (unsigned int)a;
    }
    
    if (b < 0) {
        negative = !negative;
        if (b == INT_MIN) {
            ub = (unsigned int)INT_MAX + 1;
        } else {
            ub = (unsigned int)(-b);
        }
    } else {
        ub = (unsigned int)b;
    }
    
    while (ub > 0) {
        if (ub & 1) {
            if (ua > (unsigned int)INT_MAX - result) {
                *overflow = true;
                return 0;
            }
            result += (int)ua;
        }
        ub >>= 1;
        if (ub > 0) {
            if (ua > (unsigned int)INT_MAX / 2) {
                *overflow = true;
                return 0;
            }
            ua += ua;
        }
    }
    
    if (negative) {
        if ((unsigned int)result > (unsigned int)INT_MAX + 1) {
            *overflow = true;
            return 0;
        }
        result = -result;
    } else {
        if (result < 0) {
            *overflow = true;
            return 0;
        }
    }
    
    return result;
}

int main(void) {
    int a, b;
    bool overflow;
    int result;
    
    printf("Enter two integers: ");
    if (scanf("%d %d", &a, &b) != 2) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    result = multiply(a, b, &overflow);
    
    if (overflow) {
        printf("Overflow occurred\n");
    } else {
        printf("Result: %d\n", result);
    }
    
    return 0;
}