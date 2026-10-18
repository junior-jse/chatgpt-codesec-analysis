#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shortestCommonSupersequenceLength(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) {
        return -1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len1 = strlen(str1);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len2 = strlen(str2);
    
    if (len1 == 0) {
        return (int)len2;
    }
    if (len2 == 0) {
        return (int)len1;
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

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char str1[1024];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char str2[1024];
    
    printf("Enter first string: ");
    if (fgets(str1, sizeof(str1), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter second string: ");
    if (fgets(str2, sizeof(str2), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len1 = strlen(str1);
    if (len1 > 0 && str1[len1 - 1] == '\n') {
        str1[len1 - 1] = '\0';
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len2 = strlen(str2);
    if (len2 > 0 && str2[len2 - 1] == '\n') {
        str2[len2 - 1] = '\0';
    }
    
    int result = shortestCommonSupersequenceLength(str1, str2);
    
    if (result < 0) {
        fprintf(stderr, "Error computing result\n");
        return EXIT_FAILURE;
    }
    
    printf("Length of shortest common supersequence: %d\n", result);
    
    return EXIT_SUCCESS;
}