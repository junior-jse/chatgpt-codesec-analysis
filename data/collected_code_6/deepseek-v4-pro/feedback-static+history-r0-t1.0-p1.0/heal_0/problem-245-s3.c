#include <stdio.h>
#include <stdlib.h>

int max_sum_bitonic(int arr[], int n) {
    if (n == 0) return 0;
    
    int *inc = (int *)malloc(n * sizeof(int));
    int *dec = (int *)malloc(n * sizeof(int));
    
    if (inc == NULL || dec == NULL) {
        free(inc);
        free(dec);
        return 0;
    }
    
    for (int i = 0; i < n; i++) {
        inc[i] = arr[i];
        dec[i] = arr[i];
    }
    
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[i] > arr[j] && inc[i] < inc[j] + arr[i]) {
                inc[i] = inc[j] + arr[i];
            }
        }
    }
    
    for (int i = n - 2; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            if (arr[i] > arr[j] && dec[i] < dec[j] + arr[i]) {
                dec[i] = dec[j] + arr[i];
            }
        }
    }
    
    int max_sum = 0;
    for (int i = 0; i < n; i++) {
        int sum = inc[i] + dec[i] - arr[i];
        if (sum > max_sum) {
            max_sum = sum;
        }
    }
    
    free(inc);
    free(dec);
    return max_sum;
}

int main() {
    int arr[] = {1, 15, 51, 45, 33, 100, 12, 18, 9};
    int n = sizeof(arr) / sizeof(arr[0]);
    int result = max_sum_bitonic(arr, n);
    printf("%d\n", result);
    return 0;
}