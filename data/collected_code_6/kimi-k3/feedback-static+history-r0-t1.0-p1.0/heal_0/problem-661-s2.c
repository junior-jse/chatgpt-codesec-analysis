#include <stdio.h>
#include <stdlib.h>

int max_sum_no_three_consecutive(int arr[], int n) {
    if (n <= 0) {
        return 0;
    }
    
    if (n == 1) {
        return arr[0];
    }
    
    int *dp = (int *)malloc((n + 1) * sizeof(int));
    if (dp == NULL) {
        return -1;
    }
    
    dp[0] = 0;
    dp[1] = arr[0];
    dp[2] = arr[0] + arr[1];
    
    for (int i = 3; i <= n; i++) {
        int exclude_current = dp[i - 1];
        int include_current_exclude_prev = dp[i - 2] + arr[i - 1];
        int include_current_include_prev = dp[i - 3] + arr[i - 1] + arr[i - 2];
        
        int max = exclude_current;
        if (include_current_exclude_prev > max) {
            max = include_current_exclude_prev;
        }
        if (include_current_include_prev > max) {
            max = include_current_include_prev;
        }
        
        dp[i] = max;
    }
    
    int result = dp[n];
    free(dp);
    dp = NULL;
    
    return result;
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int n = sizeof(arr) / sizeof(arr[0]);
    
    int result = max_sum_no_three_consecutive(arr, n);
    
    if (result >= 0) {
        printf("Maximum sum with no three consecutive elements: %d\n", result);
    } else {
        printf("Error occurred during computation\n");
    }
    
    return 0;
}