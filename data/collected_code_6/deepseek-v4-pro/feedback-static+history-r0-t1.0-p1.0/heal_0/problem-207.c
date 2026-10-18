#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int max_int(int a, int b) {
    return a > b ? a : b;
}

int longest_repeating_subsequence(const char *str) {
    int n;
    int **dp;
    int i;
    int j;
    int result;

    if (str == NULL) {
        return 0;
    }

    n = (int)strlen(str);
    if (n == 0) {
        return 0;
    }

    dp = (int **)malloc((n + 1) * sizeof(int *));
    if (dp == NULL) {
        return 0;
    }

    for (i = 0; i <= n; i++) {
        dp[i] = (int *)malloc((n + 1) * sizeof(int));
        if (dp[i] == NULL) {
            int k;
            for (k = 0; k < i; k++) {
                free(dp[k]);
            }
            free(dp);
            return 0;
        }
    }

    for (i = 0; i <= n; i++) {
        for (j = 0; j <= n; j++) {
            dp[i][j] = 0;
        }
    }

    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            if (str[i - 1] == str[j - 1] && i != j) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max_int(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    result = dp[n][n];

    for (i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    return result;
}

int main(void) {
    const char *test1 = "aabebcdd";
    const char *test2 = "aabb";
    const char *test3 = "axxxy";
    const char *test4 = "";
    const char *test5 = NULL;

    printf("%d\n", longest_repeating_subsequence(test1));
    printf("%d\n", longest_repeating_subsequence(test2));
    printf("%d\n", longest_repeating_subsequence(test3));
    printf("%d\n", longest_repeating_subsequence(test4));
    printf("%d\n", longest_repeating_subsequence(test5));

    return 0;
}