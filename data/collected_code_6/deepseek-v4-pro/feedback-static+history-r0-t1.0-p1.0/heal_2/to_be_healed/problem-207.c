/* Repair history (previous round):
 * - Flawfinder strlen was removed from longest_repeating_subsequence() and now appears in main().
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int max_int(int a, int b) {
    return a > b ? a : b;
}

int longest_repeating_subsequence(const char *str, size_t len) {
    size_t n;
    int **dp;
    size_t i;
    size_t j;
    int result;

    if (str == NULL) {
        return 0;
    }

    n = len;
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
            size_t k;
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

    result = (int)dp[n][n];

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

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    printf("%d\n", longest_repeating_subsequence(test1, test1 ? strlen(test1) : 0));
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    printf("%d\n", longest_repeating_subsequence(test2, test2 ? strlen(test2) : 0));
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    printf("%d\n", longest_repeating_subsequence(test3, test3 ? strlen(test3) : 0));
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    printf("%d\n", longest_repeating_subsequence(test4, test4 ? strlen(test4) : 0));
    printf("%d\n", longest_repeating_subsequence(test5, 0));

    return 0;
}