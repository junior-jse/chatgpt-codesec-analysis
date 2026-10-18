#include <stdio.h>
#include <stdlib.h>

int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int newman_shanks_williams_prime(int n) {
    if (n <= 0) return -1;
    
    long long *a = NULL;
    int count = 0;
    int result = -1;
    
    a = (long long*)malloc(3 * sizeof(long long));
    if (a == NULL) {
        return -1;
    }
    
    a[0] = 1;
    a[1] = 1;
    
    int index = 2;
    int current_capacity = 3;
    
    while (count < n) {
        if (index >= current_capacity) {
            current_capacity *= 2;
            long long *temp = (long long*)realloc(a, current_capacity * sizeof(long long));
            if (temp == NULL) {
                free(a);
                return -1;
            }
            a = temp;
        }
        
        a[index] = 2 * a[index - 1] + a[index - 2];
        
        if (is_prime(a[index])) {
            count++;
            result = a[index];
        }
        
        index++;
    }
    
    free(a);
    return result;
}

int main(void) {
    int n;
    printf("Enter n: ");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    int result = newman_shanks_williams_prime(n);
    
    if (result == -1) {
        printf("Error computing result\n");
        return 1;
    }
    
    printf("The %dth Newman-Shanks-Williams prime is: %d\n", n, result);
    
    return 0;
}