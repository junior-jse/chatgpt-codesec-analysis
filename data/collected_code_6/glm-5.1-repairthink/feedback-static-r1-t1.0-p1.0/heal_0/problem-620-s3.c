#include <stdio.h>
#include <stdlib.h>

int find_largest_divisible_subset(int *arr, int n) {
    if (n == 0) {
        return 0;
    }

    int *dp = (int *)malloc(n * sizeof(int));
    if (dp == NULL) {
        return 0;
    }

    int max_len = 1;

    for (int i = 0; i < n; ++i) {
        dp[i] = 1;
    }

    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (arr[i] % arr[j] == 0 && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }
        if (dp[i] > max_len) {
            max_len = dp[i];
        }
    }

    free(dp);
    dp = NULL;

    return max_len;
}

int compare_ints(const void *a, const void *b) {
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

int main() {
    int arr[] = {1, 2, 3, 6, 8, 16, 24};
    int n = sizeof(arr) / sizeof(arr[0]);

    qsort(arr, n, sizeof(int), compare_ints);

    int result = find_largest_divisible_subset(arr, n);

    printf("%d\n", result);

    return 0;
}