#include <stdio.h>
#include <stdbool.h>
#include <math.h>

bool is_woodall(int n) {
    if (n <= 0) {
        return false;
    }
    
    if (n == 1) {
        return true;
    }
    
    int m = 1;
    while (m <= n) {
        double val = (double)m * pow(2.0, (double)m) - 1.0;
        if (val > (double)n) {
            return false;
        }
        if ((int)val == n) {
            return true;
        }
        m++;
    }
    
    return false;
}

int main(void) {
    int test_cases[] = {1, 4, 7, 15, 63, 383, 25, 32, 49, 100};
    size_t num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (size_t i = 0; i < num_tests; i++) {
        if (is_woodall(test_cases[i])) {
            printf("%d is a Woodall number\n", test_cases[i]);
        } else {
            printf("%d is not a Woodall number\n", test_cases[i]);
        }
    }
    
    return 0;
}