#include <stdio.h>
#include <stdlib.h>

int count_sequences(int *values, int num_values, int length) {
    int *dp = (int *)calloc((length + 1) * (length + 1), sizeof(int));
    if (!dp) return -1;

    dp[0 * (length + 1) + 0] = 1;

    for (int i = 1; i <= length; i++) {
        for (int j = 0; j <= length; j++) {
            dp[i * (length + 1) + j] = 0;
            for (int k = 0; k < num_values; k++) {
                int prev = j - values[k];
                if (prev >= 0 && prev <= length) {
                    dp[i * (length + 1) + j] += dp[(i - 1) * (length + 1) + prev];
                }
            }
        }
    }

    int result = 0;
    for (int j = 0; j <= length; j++) {
        result += dp[length * (length + 1) + j];
    }

    free(dp);
    return result;
}

int main(void) {
    int values[] = {1, -1};
    int num_values = 2;
    int length = 3;
    printf("%d\n", count_sequences(values, num_values, length));
    return 0;
}