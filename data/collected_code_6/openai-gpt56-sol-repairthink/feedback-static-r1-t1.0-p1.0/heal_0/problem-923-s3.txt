#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int shortest_common_supersequence_length(const char *str1,
                                                 const char *str2,
                                                 size_t *result)
{
    size_t len1;
    size_t len2;
    size_t *dp;
    size_t i;
    size_t j;

    if (str1 == NULL || str2 == NULL || result == NULL) {
        return -1;
    }

    len1 = strlen(str1);
    len2 = strlen(str2);

    if (len2 == SIZE_MAX || len2 + 1 > SIZE_MAX / sizeof(*dp)) {
        return -1;
    }

    dp = malloc((len2 + 1) * sizeof(*dp));
    if (dp == NULL) {
        return -1;
    }

    for (j = 0; j <= len2; ++j) {
        dp[j] = j;
    }

    for (i = 1; i <= len1; ++i) {
        size_t diagonal = dp[0];

        dp[0] = i;

        for (j = 1; j <= len2; ++j) {
            size_t above = dp[j];

            if (str1[i - 1] == str2[j - 1]) {
                dp[j] = diagonal + 1;
            } else {
                size_t minimum = dp[j - 1] < above ? dp[j - 1] : above;
                dp[j] = minimum + 1;
            }

            diagonal = above;
        }
    }

    *result = dp[len2];
    free(dp);
    return 0;
}

int main(void)
{
    char str1[4096];
    char str2[4096];
    size_t result;

    if (fgets(str1, sizeof(str1), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    if (strchr(str1, '\n') == NULL && !feof(stdin)) {
        return EXIT_FAILURE;
    }
    str1[strcspn(str1, "\r\n")] = '\0';

    if (fgets(str2, sizeof(str2), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    if (strchr(str2, '\n') == NULL && !feof(stdin)) {
        return EXIT_FAILURE;
    }
    str2[strcspn(str2, "\r\n")] = '\0';

    if (shortest_common_supersequence_length(str1, str2, &result) != 0) {
        return EXIT_FAILURE;
    }

    if (printf("%zu\n", result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}