#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

int multiply(int a, int b, int *result) {
    int negative = 0;
    long long sum = 0;
    
    if (result == NULL) {
        return -1;
    }
    
    if (a == 0 || b == 0) {
        *result = 0;
        return 0;
    }
    
    if ((a < 0) != (b < 0)) {
        negative = 1;
    }
    
    long long abs_a = (a < 0) ? -(long long)a : (long long)a;
    long long abs_b = (b < 0) ? -(long long)b : (long long)b;
    
    while (abs_b > 0) {
        if (abs_b & 1) {
            sum += abs_a;
            if (sum > INT_MAX) {
                errno = ERANGE;
                return -1;
            }
        }
        abs_a <<= 1;
        abs_b >>= 1;
    }
    
    if (negative) {
        sum = -sum;
        if (sum < INT_MIN) {
            errno = ERANGE;
            return -1;
        }
    }
    
    *result = (int)sum;
    return 0;
}

int main(void) {
    int a, b, result;
    
    printf("Enter two integers: ");
    if (scanf("%d %d", &a, &b) != 2) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (multiply(a, b, &result) != 0) {
        fprintf(stderr, "Multiplication overflow\n");
        return EXIT_FAILURE;
    }
    
    printf("%d * %d = %d\n", a, b, result);
    
    return EXIT_SUCCESS;
}