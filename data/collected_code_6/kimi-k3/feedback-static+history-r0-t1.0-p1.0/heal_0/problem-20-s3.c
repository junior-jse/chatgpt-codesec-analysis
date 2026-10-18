#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

bool is_woodall(unsigned long long n)
{
    if (n < 7ULL) {
        return false;
    }
    
    unsigned long long temp = n;
    unsigned long long k = 0ULL;
    
    while (temp % 2ULL == 0ULL) {
        temp /= 2ULL;
        k++;
    }
    
    if (temp == 1ULL) {
        return false;
    }
    
    if (temp % 2ULL == 0ULL) {
        return false;
    }
    
    unsigned long long m = (temp + 1ULL) / 2ULL;
    
    if (m == 0ULL) {
        return false;
    }
    
    if (m > ULLONG_MAX / k) {
        return false;
    }
    
    unsigned long long check = m * k;
    
    if (check > ULLONG_MAX - 1ULL) {
        return false;
    }
    
    return (check + 1ULL) == temp;
}

int main(void)
{
    unsigned long long test_numbers[] = {7ULL, 23ULL, 63ULL, 159ULL, 383ULL, 1ULL, 8ULL, 64ULL};
    size_t count = sizeof(test_numbers) / sizeof(test_numbers[0]);
    
    for (size_t i = 0; i < count; i++) {
        if (is_woodall(test_numbers[i])) {
            printf("%llu is a Woodall number\n", test_numbers[i]);
        } else {
            printf("%llu is not a Woodall number\n", test_numbers[i]);
        }
    }
    
    return 0;
}