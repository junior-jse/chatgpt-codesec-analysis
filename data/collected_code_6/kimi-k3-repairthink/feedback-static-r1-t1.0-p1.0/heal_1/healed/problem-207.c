#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 4096

static size_t bounded_strlen(const char *str, size_t max_len)
{
    size_t len = 0;

    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int longestRepeatingSubsequence(const char *str)
{
    if (str == NULL) {
        return -1;
    }

    size_t len = bounded_strlen(str, (size_t)MAX_STRING_LENGTH + 1);
    if (len == 0) {
        return 0;
    }
    if (len > MAX_STRING_LENGTH) {
        return -1;
    }

    int *dp = calloc((len + 1) * (len + 1), sizeof(int));
    if (dp == NULL) {
        return -1;
    }

    int maxLen = 0;

    for (size_t i = 1; i <= len; i++) {
        for (size_t j = 1; j <= len; j++) {
            size_t idx = i * (len + 1) + j;
            size_t prevDiag = (i - 1) * (len + 1) + (j - 1);
            size_t prevUp = (i - 1) * (len + 1) + j;
            size_t prevLeft = i * (len + 1) + (j - 1);

            if (str[i - 1] == str[j - 1] && i != j) {
                dp[idx] = dp[prevDiag] + 1;
            } else {
                dp[idx] = (dp[prevUp] > dp[prevLeft]) ? dp[prevUp] : dp[prevLeft];
            }

            if (dp[idx] > maxLen) {
                maxLen = dp[idx];
            }
        }
    }

    free(dp);
    return maxLen;
}

int main(void)
{
    const char *test1 = "aabb";
    const char *test2 = "aab";
    const char *test3 = "aabebcdd";
    const char *test4 = "";

    int result;

    result = longestRepeatingSubsequence(test1);
    if (result >= 0) {
        printf("Input: \"%s\" -> Longest Repeating Subsequence: %d\n", test1, result);
    }

    result = longestRepeatingSubsequence(test2);
    if (result >= 0) {
        printf("Input: \"%s\" -> Longest Repeating Subsequence: %d\n", test2, result);
    }

    result = longestRepeatingSubsequence(test3);
    if (result >= 0) {
        printf("Input: \"%s\" -> Longest Repeating Subsequence: %d\n", test3, result);
    }

    result = longestRepeatingSubsequence(test4);
    if (result >= 0) {
        printf("Input: \"%s\" -> Longest Repeating Subsequence: %d\n", test4, result);
    }

    return 0;
}