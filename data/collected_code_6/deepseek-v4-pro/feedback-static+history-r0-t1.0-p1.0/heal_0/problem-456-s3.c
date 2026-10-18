#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 100
#define MAX_LENGTH 256

void reverse_string(char *str) {
    if (str == NULL) {
        return;
    }
    
    int len = strlen(str);
    if (len <= 0) {
        return;
    }
    
    for (int i = 0; i < len / 2; i++) {
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
            reverse_string(strings[i]);
        }
    }
}

int main(void) {
    char *strings[MAX_STRINGS] = {NULL};
    int count = 0;
    char buffer[MAX_LENGTH];
    
    printf("Enter number of strings (max %d): ", MAX_STRINGS);
    if (scanf("%d", &count) != 1 || count <= 0 || count > MAX_STRINGS) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    while (getchar() != '\n');
    
    for (int i = 0; i < count; i++) {
        printf("Enter string %d: ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            for (int j = 0; j < i; j++) {
                free(strings[j]);
            }
            return EXIT_FAILURE;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        strings[i] = malloc(len + 1);
        if (strings[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(strings[j]);
            }
            return EXIT_FAILURE;
        }
        strcpy(strings[i], buffer);
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