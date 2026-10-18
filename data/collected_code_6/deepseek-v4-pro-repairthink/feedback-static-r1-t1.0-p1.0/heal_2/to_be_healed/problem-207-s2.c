#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING_LEN 1000

int longest_repeating_subsequence(const char *str) {
    if (str == NULL) return 0;
    size_t n = 0;
    /* Possible weaknesses found:
     *  Assuming that condition 'n<1000' is not redundant
     *  Array index out of bounds
     *  Either the condition 'n<1000' is redundant or the array 'str[1]' is accessed at index 999, which is out of bounds. [arrayIndexOutOfBoundsCond]
     */
    while (n < MAX_STRING_LEN && str[n] != '\0') n++;
    if (n == 0) return 0;
    int *dp = calloc((n + 1) * (n + 1), sizeof(int));
    if (dp == NULL) return 0;
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = 1; j <= n; j++) {
            if (str[i - 1] == str[j - 1] && i != j) {
                dp[i * (n + 1) + j] = dp[(i - 1) * (n + 1) + (j - 1)] + 1;
            } else {
                int up = dp[(i - 1) * (n + 1) + j];
                int left = dp[i * (n + 1) + (j - 1)];
                dp[i * (n + 1) + j] = (up > left) ? up : left;
            }
        }
    }
    int result = dp[n * (n + 1) + n];
    free(dp);
    return result;
}

int main(void) {
    const char *test1 = "AABEBCDD";
    const char *test2 = "axxxy";
    const char *test3 = "aab";
    const char *test4 = "abc";
    const char *test5 = "";

    printf("Longest repeating subsequence of %s: %d\n", test1, longest_repeating_subsequence(test1));
    printf("Longest repeating subsequence of %s: %d\n", test2, longest_repeating_subsequence(test2));
    printf("Longest repeating subsequence of %s: %d\n", test3, longest_repeating_subsequence(test3));
    printf("Longest repeating subsequence of %s: %d\n", test4, longest_repeating_subsequence(test4));
    printf("Longest repeating subsequence of %s: %d\n", test5, longest_repeating_subsequence(test5));

    return 0;
}