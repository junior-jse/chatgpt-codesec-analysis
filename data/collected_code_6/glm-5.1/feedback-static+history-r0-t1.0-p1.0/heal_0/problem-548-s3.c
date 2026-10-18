#include <stdio.h>
#include <stdlib.h>

int longest_increasing_subsequence(const int *sequence, size_t length) {
    if (sequence == NULL || length == 0) {
        return 0;
    }

    size_t *dp = (size_t *)malloc(length * sizeof(size_t));
    if (dp == NULL) {
        return 0;
    }

    size_t max_length = 1;
    for (size_t i = 0; i < length; i++) {
        dp[i] = 1;
        for (size_t j = 0; j < i; j++) {
            if (sequence[j] < sequence[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
            }
        }
        if (dp[i] > max_length) {
            max_length = dp[i];
        }
    }

    free(dp);
    return (int)max_length;
}

int main(void) {
    int sequence[] = {10, 22, 9, 33, 21, 50, 41, 60};
    size_t length = sizeof(sequence) / sizeof(sequence[0]);

    int result = longest_increasing_subsequence(sequence, length);
    printf("%d\n", result);

    return 0;
}