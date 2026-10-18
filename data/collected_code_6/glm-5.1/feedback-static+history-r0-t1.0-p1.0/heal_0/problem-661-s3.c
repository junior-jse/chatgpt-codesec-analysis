#include <stdio.h>
#include <stdlib.h>

int max_sum_no_three_consecutive(int *arr, int n) {
    if (arr == NULL || n <= 0) {
        return 0;
    }
    if (n == 1) {
        return arr[0] > 0 ? arr[0] : 0;
    }
    if (n == 2) {
        int sum = arr[0] + arr[1];
        return sum > 0 ? sum : 0;
    }

    int *dp = (int *)malloc(n * sizeof(int));
    if (dp == NULL) {
        return 0;
    }

    dp[0] = arr[0] > 0 ? arr[0] : 0;
    dp[1] = (arr[0] + arr[1]) > 0 ? (arr[0] + arr[1]) : 0;
    dp[2] = (arr[0] + arr[1]) > (arr[1] + arr[2]) ? (arr[0] + arr[1]) : (arr[1] + arr[2]);
    dp[2] = dp[2] > arr[0] ? dp[2] : arr[0];
    if (arr[2] > dp[2]) dp[2] = arr[2];
    if (arr[1] > dp[2]) dp[2] = arr[1];
    dp[2] = dp[2] > 0 ? dp[2] : 0;

    for (int i = 3; i < n; i++) {
        int val1 = dp[i - 1];
        int val2 = dp[i - 2] + arr[i];
        int val3 = dp[i - 3] + arr[i - 1] + arr[i];
        
        int max_val = val1 > val2 ? val1 : val2;
        max_val = max_val > val3 ? max_val : val3;
        
        dp[i] = max_val > 0 ? max_val : 0;
    }

    int result = dp[n - 1];
    free(dp);
    
    return result;
}

int main() {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("%d\n", max_sum_no_three_consecutive(arr1, n1));

    int arr2[] = {5, 5, 10, 100, 10, 5};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("%d\n", max_sum_no_three_consecutive(arr2, n2));

    int arr3[] = {3, 2, 7, 10};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    printf("%d\n", max_sum_no_three_consecutive(arr3, n3));

    int *arr4 = NULL;
    printf("%d\n", max_sum_no_three_consecutive(arr4, 0));

    return 0;
}