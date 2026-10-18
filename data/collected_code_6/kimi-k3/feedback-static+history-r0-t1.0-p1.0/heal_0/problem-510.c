#include <stdio.h>
#include <stdlib.h>

long long countSubsequences(int arr[], int n, long long k) {
    if (k <= 1) {
        return 0;
    }
    
    long long count = 0;
    
    for (int i = 0; i < n; i++) {
        if (arr[i] < k) {
            count++;
            long long product = arr[i];
            
            for (int j = i + 1; j < n; j++) {
                if (arr[j] != 0 && product > (k - 1) / arr[j]) {
                    break;
                }
                product *= arr[j];
                if (product < k) {
                    count++;
                } else {
                    break;
                }
            }
        }
    }
    
    return count;
}

int main() {
    int n;
    long long k;
    
    if (scanf("%d", &n) != 1 || n <= 0) {
        return 1;
    }
    
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        return 1;
    }
    
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1 || arr[i] < 0) {
            free(arr);
            return 1;
        }
    }
    
    if (scanf("%lld", &k) != 1 || k < 0) {
        free(arr);
        return 1;
    }
    
    long long result = countSubsequences(arr, n, k);
    printf("%lld\n", result);
    
    free(arr);
    return 0;
}