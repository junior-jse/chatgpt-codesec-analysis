#include <stdio.h>
#include <stdbool.h>

bool is_woodall(int n) {
    if (n < 0) {
        return false;
    }
    
    if (n % 2 == 0) {
        return false;
    }
    
    for (int m = 0; m < 30; m++) {
        long long woodall = (long long)m * (1LL << m) - 1;
        if (woodall == n) {
            return true;
        }
        if (woodall > n) {
            break;
        }
    }
    
    return false;
}

int main(void) {
    int number;
    
    printf("Enter a number: ");
    if (scanf("%d", &number) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (is_woodall(number)) {
        printf("%d is a Woodall number\n", number);
    } else {
        printf("%d is not a Woodall number\n", number);
    }
    
    return 0;
}