#include <stdio.h>
#include <stdlib.h>

int max_sum_no_three_consecutive(int* arr, int n) {
    if (arr == NULL || n <= 0) {
        return 0;
    }
    if (n == 1) {
        return arr[0];
    }
    if (n == 2) {
        return arr[0] + arr[1];
    }

    int* dp = (int*)malloc(n * sizeof(int));
    if (dp == NULL) {
        exit(EXIT_FAILURE);
    }

    dp[0] = arr[0];
    dp[1] = arr[0] + arr[1];
    dp[2] = (arr[0] + arr[1] > arr[1] + arr[2]) ? (arr[0] + arr[1] > arr[0] + arr[2] ? arr[0] + arr[1] : arr[0] + arr[2]) : (arr[1] + arr[2] > arr[0] + arr[2] ? arr[1] + arr[2] : arr[0] + arr[2]);

    for (int i = 3; i < n; i++) {
        int a = dp[i - 1];
        int b = dp[i - 2] + arr[i];
        int c = dp[i - 3] + arr[i - 1] + arr[i];
        int max_bc = b > c ? b : c;
        dp[i] = a > max_bc ? a : max_bc;
    }

    int result = dp[n - 1];
    free(dp);
    
    return result;
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int n = sizeof(arr) / sizeof(arr[0]);
    int max_sum = max_sum_no_three_consecutive(arr, n);
    printf("%d\n", max_sum);
    return 0;
}