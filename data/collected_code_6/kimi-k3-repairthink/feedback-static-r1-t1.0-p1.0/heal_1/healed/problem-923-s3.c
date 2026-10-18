#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_SIZE 1024

static size_t boundedStringLength(const char *s, size_t maxLen)
{
    size_t len = 0;

    if (s == NULL) {
        return 0;
    }

    while (len < maxLen && s[len] != '\0') {
        len++;
    }

    return len;
}

int shortestCommonSupersequenceLength(const char *str1, size_t len1, const char *str2, size_t len2)
{
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }

    if (len1 == 0) {
        if (len2 > (size_t)INT32_MAX) {
            return -1;
        }
        return (int)len2;
    }
    if (len2 == 0) {
        if (len1 > (size_t)INT32_MAX) {
            return -1;
        }
        return (int)len1;
    }

    if (len2 + 1 > SIZE_MAX / sizeof(size_t)) {
        return -1;
    }

    size_t *prev = (size_t *)calloc(len2 + 1, sizeof(size_t));
    if (prev == NULL) {
        return -1;
    }

    size_t *curr = (size_t *)calloc(len2 + 1, sizeof(size_t));
    if (curr == NULL) {
        free(prev);
        return -1;
    }

    for (size_t j = 0; j <= len2; j++) {
        prev[j] = j;
    }

    for (size_t i = 1; i <= len1; i++) {
        curr[0] = i;
        for (size_t j = 1; j <= len2; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                size_t del = prev[j] + 1;
                size_t ins = curr[j - 1] + 1;
                curr[j] = (del < ins) ? del : ins;
            }
        }
        size_t *temp = prev;
        prev = curr;
        curr = temp;
    }

    size_t result = prev[len2];

    free(prev);
    free(curr);

    if (result > (size_t)INT32_MAX) {
        return -1;
    }

    return (int)result;
}

int main(void)
{
    char str1[MAX_INPUT_SIZE];
    char str2[MAX_INPUT_SIZE];

    printf("Enter first string: ");
    if (fgets(str1, sizeof(str1), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    str1[sizeof(str1) - 1] = '\0';

    printf("Enter second string: ");
    if (fgets(str2, sizeof(str2), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    str2[sizeof(str2) - 1] = '\0';

    size_t len1 = boundedStringLength(str1, sizeof(str1));
    if (len1 > 0 && str1[len1 - 1] == '\n') {
        str1[len1 - 1] = '\0';
        len1--;
    } else if (len1 == sizeof(str1) - 1) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }

    size_t len2 = boundedStringLength(str2, sizeof(str2));
    if (len2 > 0 && str2[len2 - 1] == '\n') {
        str2[len2 - 1] = '\0';
        len2--;
    } else if (len2 == sizeof(str2) - 1) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }

    int result = shortestCommonSupersequenceLength(str1, len1, str2, len2);

    if (result < 0) {
        fprintf(stderr, "Error computing result\n");
        return EXIT_FAILURE;
    }

    printf("Length of shortest common supersequence: %d\n", result);

    return EXIT_SUCCESS;
}