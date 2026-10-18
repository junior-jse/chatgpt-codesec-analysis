#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static int max_segments(int n, int a, int b, int c)
{
    if (n < 0 || a <= 0 || b <= 0 || c <= 0) {
        return -1;
    }

    int *dp = malloc(((size_t)n + 1U) * sizeof(*dp));
    if (dp == NULL) {
        return -1;
    }

    dp[0] = 0;

    for (int i = 1; i <= n; ++i) {
        dp[i] = INT_MIN;

        if (i >= a && dp[i - a] != INT_MIN && dp[i - a] < INT_MAX) {
            dp[i] = dp[i - a] + 1;
        }

        if (i >= b && dp[i - b] != INT_MIN && dp[i - b] < INT_MAX &&
            dp[i - b] + 1 > dp[i]) {
            dp[i] = dp[i - b] + 1;
        }

        if (i >= c && dp[i - c] != INT_MIN && dp[i - c] < INT_MAX &&
            dp[i - c] + 1 > dp[i]) {
            dp[i] = dp[i - c] + 1;
        }
    }

    int result = dp[n] == INT_MIN ? 0 : dp[n];
    free(dp);

    return result;
}

int main(void)
{
    int n;
    int a;
    int b;
    int c;

    if (scanf("%d %d %d %d", &n, &a, &b, &c) != 4) {
        return EXIT_FAILURE;
    }

    int result = max_segments(n, a, b, c);
    if (result < 0) {
        return EXIT_FAILURE;
    }

    if (printf("%d\n", result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}