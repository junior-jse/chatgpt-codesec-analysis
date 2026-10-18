#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 4096

int shortestCommonSupersequenceLength(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }

    size_t len1 = strnlen(str1, MAX_INPUT_LENGTH);
    size_t len2 = strnlen(str2, MAX_INPUT_LENGTH);

    if (len1 == 0) {
        return (int)len2;
    }
    if (len2 == 0) {
        return (int)len1;
    }

    size_t **dp = malloc((len1 + 1) * sizeof(size_t *));
    if (dp == NULL) {
        return -1;
    }

    for (size_t i = 0; i <= len1; i++) {
        dp[i] = malloc((len2 + 1) * sizeof(size_t));
        if (dp[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return -1;
        }
    }

    for (size_t i = 0; i <= len1; i++) {
        dp[i][0] = i;
    }
    for (size_t j = 0; j <= len2; j++) {
        dp[0][j] = j;
    }

    for (size_t i = 1; i <= len1; i++) {
        for (size_t j = 1; j <= len2; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                size_t option1 = dp[i - 1][j] + 1;
                size_t option2 = dp[i][j - 1] + 1;
                dp[i][j] = (option1 < option2) ? option1 : option2;
            }
        }
    }

    size_t result = dp[len1][len2];

    for (size_t i = 0; i <= len1; i++) {
        free(dp[i]);
    }
    free(dp);

    return (int)result;
}

int main(void) {
    const char *str1 = "AGGTAB";
    const char *str2 = "GXTXAYB";

    int result = shortestCommonSupersequenceLength(str1, str2);

    if (result >= 0) {
        printf("Length of shortest common supersequence: %d\n", result);
    } else {
        printf("Error occurred during computation\n");
        return 1;
    }

    return 0;
}