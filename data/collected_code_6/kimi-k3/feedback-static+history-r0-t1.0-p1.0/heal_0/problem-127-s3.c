#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <errno.h>

int safe_multiply(int a, int b, int *result) {
    if (result == NULL) {
        return -1;
    }
    
    if (a == 0 || b == 0) {
        *result = 0;
        return 0;
    }
    
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) {
                return -1;
            }
        } else {
            if (b < INT_MIN / a) {
                return -1;
            }
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) {
                return -1;
            }
        } else {
            if (a < INT_MAX / b) {
                return -1;
            }
        }
    }
    
    int abs_a = (a < 0) ? -a : a;
    int abs_b = (b < 0) ? -b : b;
    int product = 0;
    
    while (abs_b > 0) {
        if (abs_b & 1) {
            product += abs_a;
        }
        abs_a <<= 1;
        abs_b >>= 1;
    }
    
    if ((a < 0) != (b < 0)) {
        product = -product;
    }
    
    *result = product;
    return 0;
}

int main(void) {
    int x, y, result;
    
    printf("Enter two integers: ");
    if (scanf("%d %d", &x, &y) != 2) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (safe_multiply(x, y, &result) != 0) {
        fprintf(stderr, "Multiplication overflow\n");
        return 1;
    }
    
    printf("Result: %d\n", result);
    return 0;
}