#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>

char findMaxOccurringChar(const char *str, int *maxCount) {
    uint32_t freq[UCHAR_MAX + 1] = {0};
    size_t i;
    int max = 0;
    char result = '\0';
    
    if (str == NULL || maxCount == NULL) {
        if (maxCount != NULL) {
            *maxCount = 0;
        }
        return '\0';
    }
    
    for (i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        freq[c]++;
    }
    
    for (i = 0; i <= UCHAR_MAX; i++) {
        if (freq[i] > (uint32_t)max) {
            max = (int)freq[i];
            result = (char)i;
        }
    }
    
    *maxCount = max;
    return result;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[1024];
    int maxCount = 0;
    char maxChar;
    size_t len;
    int c;
    
    printf("Enter a string: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len = strlen(buffer);
    
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == sizeof(buffer) - 1) {
        /* Possible weaknesses found:
         * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    if (buffer[0] == '\0') {
        printf("Empty string provided\n");
        return 0;
    }
    
    maxChar = findMaxOccurringChar(buffer, &maxCount);
    
    if (maxCount > 0) {
        printf("Maximum occurring character: '%c' (appears %d times)\n", maxChar, maxCount);
    } else {
        printf("No characters found\n");
    }
    
    return 0;
}