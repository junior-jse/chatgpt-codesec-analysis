#include <stdio.h>
#include <stdlib.h>

int count_sequences(int *values, int n, int k, int len, int current_sum, long long *dp) {
    if (current_sum < 0) {
        return 0;
    }
    if (len == k) {
        return 1;
    }
    int idx = len * (k + 1) + current_sum;
    if (dp[idx] != -1) {
        return (int)dp[idx];
    }
    long long total = 0;
    for (int i = 0; i < n; i++) {
        total += count_sequences(values, n, k, len + 1, current_sum + values[i], dp);
    }
    if (total > 2147483647) {
        total = 2147483647;
    }
    dp[idx] = total;
    return (int)total;
}

int count_valid_sequences(int *values, int n, int k) {
    if (n <= 0 || k <= 0) {
        return 0;
    }
    int max_sum = 0;
    for (int i = 0; i < n; i++) {
        if (values[i] > 0) {
            max_sum += values[i];
        }
    }
    max_sum *= k;
    if (max_sum > 5000) {
        max_sum = 5000;
    }
    int dp_size = (k + 1) * (max_sum + 1);
    long long *dp = (long long *)malloc((size_t)dp_size * sizeof(long long));
    if (dp == NULL) {
        return 0;
    }
    for (int i = 0; i < dp_size; i++) {
        dp[i] = -1;
    }
    int result = count_sequences(values, n, k, 0, 0, dp);
    free(dp);
    return result;
}

int main(void) {
    int values[] = {1, -1};
    int n = sizeof(values) / sizeof(values[0]);
    int k = 4;
    int result = count_valid_sequences(values, n, k);
    printf("%d\n", result);
    return 0;
}