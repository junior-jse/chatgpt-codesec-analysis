#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

char find_most_common_char(const char *str, int *count) {
    int char_count[UCHAR_MAX + 1] = {0};
    int i;
    int max_count = 0;
    char most_common = '\0';
    
    if (str == NULL || count == NULL) {
        if (count != NULL) {
            *count = 0;
        }
        return '\0';
    }
    
    for (i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        char_count[c]++;
    }
    
    for (i = 0; i <= UCHAR_MAX; i++) {
        if (char_count[i] > max_count) {
            max_count = char_count[i];
            most_common = (char)i;
        }
    }
    
    *count = max_count;
    return most_common;
}

int main(void) {
    char buffer[1024];
    int count = 0;
    char result;
    
    printf("Enter a string: ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    if (strlen(buffer) == 0) {
        fprintf(stderr, "Empty string provided\n");
        return EXIT_FAILURE;
    }
    
    result = find_most_common_char(buffer, &count);
    
    if (count > 0) {
        printf("Most common character: '%c' (appears %d times)\n", result, count);
    } else {
        printf("No characters found\n");
    }
    
    return EXIT_SUCCESS;
}