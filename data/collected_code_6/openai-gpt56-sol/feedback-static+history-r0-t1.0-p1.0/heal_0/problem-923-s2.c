#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int shortest_common_supersequence_length(const char *str1, const char *str2, size_t *result)
{
    if (str1 == NULL || str2 == NULL || result == NULL) {
        return -1;
    }

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if (len1 > SIZE_MAX - len2) {
        return -1;
    }

    if (len2 == SIZE_MAX) {
        return -1;
    }

    size_t column_count = len2 + 1;
    if (column_count > SIZE_MAX / sizeof(size_t)) {
        return -1;
    }

    size_t *dp = malloc(column_count * sizeof(*dp));
    if (dp == NULL) {
        return -1;
    }

    for (size_t j = 0; j <= len2; ++j) {
        dp[j] = 0;
    }

    for (size_t i = 1; i <= len1; ++i) {
        size_t previous_diagonal = 0;

        for (size_t j = 1; j <= len2; ++j) {
            size_t previous_row = dp[j];

            if (str1[i - 1] == str2[j - 1]) {
                dp[j] = previous_diagonal + 1;
            } else if (dp[j - 1] > dp[j]) {
                dp[j] = dp[j - 1];
            }

            previous_diagonal = previous_row;
        }
    }

    *result = len1 + len2 - dp[len2];
    free(dp);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <str1> <str2>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t length = 0;
    if (shortest_common_supersequence_length(argv[1], argv[2], &length) != 0) {
        fprintf(stderr, "Failed to compute the shortest length.\n");
        return EXIT_FAILURE;
    }

    printf("%zu\n", length);
    return EXIT_SUCCESS;
}