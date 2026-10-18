#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 1024

char* abbreviate_road(const char* input) {
    if (input == NULL) {
        return NULL;
    }
    
    size_t len = strnlen(input, MAX_INPUT_SIZE);
    char* result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }
    
    size_t i = 0;
    size_t j = 0;
    
    while (i < len) {
        if (i + 3 < len && 
            tolower((unsigned char)input[i]) == 'r' &&
            tolower((unsigned char)input[i + 1]) == 'o' &&
            tolower((unsigned char)input[i + 2]) == 'a' &&
            tolower((unsigned char)input[i + 3]) == 'd' &&
            (i == 0 || isspace((unsigned char)input[i - 1]) || ispunct((unsigned char)input[i - 1])) &&
            (i + 4 >= len || isspace((unsigned char)input[i + 4]) || ispunct((unsigned char)input[i + 4]))) {
            
            result[j++] = 'r';
            result[j++] = 'd';
            result[j++] = '.';
            i += 4;
        } else {
            result[j++] = input[i++];
        }
    }
    
    result[j] = '\0';
    
    char* final_result = realloc(result, j + 1);
    if (final_result == NULL) {
        free(result);
        return NULL;
    }
    
    return final_result;
}

int main(void) {
    char* input = malloc(MAX_INPUT_SIZE);
    if (input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter a string: ");
    if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        free(input);
        return 1;
    }
    
    size_t len = strnlen(input, MAX_INPUT_SIZE);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    char* abbreviated = abbreviate_road(input);
    if (abbreviated == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(input);
        return 1;
    }
    
    printf("Abbreviated: %s\n", abbreviated);
    
    free(abbreviated);
    free(input);
    
    return 0;
}