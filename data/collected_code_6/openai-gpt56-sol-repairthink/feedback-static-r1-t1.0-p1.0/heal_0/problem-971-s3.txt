#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static int max_segments(int n, int a, int b, int c)
{
    int *dp;
    int lengths[3] = {a, b, c};
    int result;

    if (n < 0 || a <= 0 || b <= 0 || c <= 0) {
        return -1;
    }

    dp = malloc(((size_t)n + 1U) * sizeof(*dp));
    if (dp == NULL) {
        return -1;
    }

    dp[0] = 0;

    for (int i = 1; i <= n; ++i) {
        dp[i] = INT_MIN;
    }

    for (int i = 1; i <= n; ++i) {
        for (size_t j = 0; j < 3U; ++j) {
            if (i >= lengths[j] && dp[i - lengths[j]] != INT_MIN) {
                int candidate = dp[i - lengths[j]] + 1;

                if (candidate > dp[i]) {
                    dp[i] = candidate;
                }
            }
        }
    }

    result = dp[n] == INT_MIN ? 0 : dp[n];
    free(dp);

    return result;
}

int main(void)
{
    int n;
    int a;
    int b;
    int c;
    int result;

    if (scanf("%d %d %d %d", &n, &a, &b, &c) != 4) {
        return EXIT_FAILURE;
    }

    result = max_segments(n, a, b, c);
    if (result < 0) {
        return EXIT_FAILURE;
    }

    printf("%d\n", result);
    return EXIT_SUCCESS;
}