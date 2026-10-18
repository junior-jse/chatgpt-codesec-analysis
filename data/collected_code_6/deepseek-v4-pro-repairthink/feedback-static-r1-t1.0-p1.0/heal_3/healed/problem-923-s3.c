#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

static size_t safe_strlen(const char *s, size_t maxlen) {
    size_t i = 0;
    while (i < maxlen && s[i] != '\0') {
        i++;
    }
    return i;
}

int shortest_common_supersequence_length(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }

    size_t n1 = safe_strlen(str1, INT_MAX);
    if (n1 >= INT_MAX) {
        return -1;
    }
    size_t n2 = safe_strlen(str2, INT_MAX);
    if (n2 >= INT_MAX) {
        return -1;
    }
    int len1 = (int)n1;
    int len2 = (int)n2;

    size_t rows = (size_t)len1 + 1;
    size_t cols = (size_t)len2 + 1;

    int **dp = malloc(rows * sizeof(int *));
    if (dp == NULL) {
        return -1;
    }

    if (rows > SIZE_MAX / cols) {
        free(dp);
        return -1;
    }
    int *data = calloc(rows * cols, sizeof(int));
    if (data == NULL) {
        free(dp);
        return -1;
    }

    for (size_t row = 0; row < rows; row++) {
        dp[row] = data + row * cols;
    }

    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1]) ? dp[i - 1][j] : dp[i][j - 1];
            }
        }
    }

    int lcs_length = dp[len1][len2];

    free(data);
    free(dp);

    return len1 + len2 - lcs_length;
}

int main(void) {
    const char *str1 = "AGGTAB";
    const char *str2 = "GXTXAYB";

    int result = shortest_common_supersequence_length(str1, str2);

    if (result != -1) {
        printf("Length of shortest common supersequence: %d\n", result);
    } else {
        printf("Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}