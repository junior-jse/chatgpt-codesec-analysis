#include <stdio.h>

int is_woodall(int n) {
    if (n <= 0) return 0;
    
    int m = 1;
    while (m * (1 << m) - 1 <= n) {
        if (m * (1 << m) - 1 == n) return 1;
        m++;
    }
    
    return 0;
}

int main(void) {
    int num;
    
    printf("Enter a number: ");
    if (scanf("%d", &num) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (is_woodall(num)) {
        printf("%d is a Woodall number\n", num);
    } else {
        printf("%d is not a Woodall number\n", num);
    }
    
    return 0;
}