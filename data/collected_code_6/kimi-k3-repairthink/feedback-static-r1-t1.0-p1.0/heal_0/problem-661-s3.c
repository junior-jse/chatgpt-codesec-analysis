#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int maxSumNoThreeConsecutive(int arr[], int n) {
    if (n <= 0) {
        return 0;
    }
    
    if (n == 1) {
        return arr[0];
    }
    
    if (n == 2) {
        return arr[0] + arr[1];
    }
    
    int *dp = (int *)malloc(n * sizeof(int));
    if (dp == NULL) {
        return INT_MIN;
    }
    
    dp[0] = arr[0];
    dp[1] = arr[0] + arr[1];
    dp[2] = (arr[0] + arr[1] > arr[1] + arr[2]) ? 
            ((arr[0] + arr[1] > arr[0] + arr[2]) ? arr[0] + arr[1] : arr[0] + arr[2]) :
            ((arr[1] + arr[2] > arr[0] + arr[2]) ? arr[1] + arr[2] : arr[0] + arr[2]);
    
    for (int i = 3; i < n; i++) {
        int exclude = dp[i - 1];
        int includeExclude = dp[i - 2] + arr[i];
        int includeIncludeExclude = dp[i - 3] + arr[i - 1] + arr[i];
        
        dp[i] = exclude;
        if (includeExclude > dp[i]) {
            dp[i] = includeExclude;
        }
        if (includeIncludeExclude > dp[i]) {
            dp[i] = includeIncludeExclude;
        }
    }
    
    int result = dp[n - 1];
    free(dp);
    dp = NULL;
    
    return result;
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    int result = maxSumNoThreeConsecutive(arr, n);
    
    if (result != INT_MIN) {
        printf("Maximum sum with no three consecutive elements: %d\n", result);
    } else {
        printf("Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}