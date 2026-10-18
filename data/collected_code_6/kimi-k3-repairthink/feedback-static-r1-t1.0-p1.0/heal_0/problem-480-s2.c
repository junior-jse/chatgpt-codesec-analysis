#include <stdio.h>
#include <string.h>
#include <limits.h>

char findMaxOccurringChar(const char *str, int *maxCount) {
    int freq[256] = {0};
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
    
    for (i = 0; i < 256; i++) {
        if (freq[i] > max) {
            max = freq[i];
            result = (char)i;
        }
    }
    
    *maxCount = max;
    return result;
}

int main(void) {
    char buffer[1024];
    int maxCount = 0;
    char maxChar;
    
    printf("Enter a string: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    if (strlen(buffer) == 0) {
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