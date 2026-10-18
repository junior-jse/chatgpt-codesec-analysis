#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 100
#define MAX_LENGTH 256

void reverse_string(char *str, size_t len) {
    if (str == NULL || len <= 1) {
        return;
    }
    
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

void reverse_strings(char **strings, int count) {
    if (strings == NULL || count <= 0) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        if (strings[i] != NULL) {
            size_t len = strnlen(strings[i], MAX_LENGTH);
            reverse_string(strings[i], len);
        }
    }
}

int main(void) {
    char *strings[MAX_STRINGS];
    int count = 0;
    char buffer[MAX_LENGTH];
    int c;
    
    for (int i = 0; i < MAX_STRINGS; i++) {
        strings[i] = NULL;
    }
    
    printf("Enter number of strings (max %d): ", MAX_STRINGS);
    if (scanf("%d", &count) != 1 || count <= 0 || count > MAX_STRINGS) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    while ((c = getchar()) != '\n' && c != EOF);
    
    for (int i = 0; i < count; i++) {
        printf("Enter string %d: ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            for (int j = 0; j < i; j++) {
                free(strings[j]);
            }
            return EXIT_FAILURE;
        }
        
        size_t len = strnlen(buffer, sizeof(buffer));
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            fprintf(stderr, "Empty string not allowed\n");
            for (int j = 0; j < i; j++) {
                free(strings[j]);
            }
            return EXIT_FAILURE;
        }
        
        strings[i] = malloc(len + 1);
        if (strings[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(strings[j]);
            }
            return EXIT_FAILURE;
        }
        
        if (len + 1 > MAX_LENGTH) {
            fprintf(stderr, "Input too long\n");
            free(strings[i]);
            strings[i] = NULL;
            for (int j = 0; j < i; j++) {
                free(strings[j]);
            }
            return EXIT_FAILURE;
        }
        
        strncpy(strings[i], buffer, len + 1);
        strings[i][len] = '\0';
    }
    
    reverse_strings(strings, count);
    
    printf("\nReversed strings:\n");
    for (int i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
    }
    
    for (int i = 0; i < count; i++) {
        free(strings[i]);
    }
    
    return EXIT_SUCCESS;
}