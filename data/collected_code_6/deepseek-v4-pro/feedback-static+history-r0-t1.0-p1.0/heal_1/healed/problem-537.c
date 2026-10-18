#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000
#define MAX_INPUT_LENGTH 1000

char* find_first_repeated_word(const char* str) {
    if (str == NULL || *str == '\0') {
        return NULL;
    }

    char* words[MAX_WORDS];
    char* str_copy = strdup(str);
    if (str_copy == NULL) {
        return NULL;
    }

    int word_count = 0;
    char* token = strtok(str_copy, " \t\n\r.,;:!?\"'()[]{}");
    
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len >= MAX_WORD_LENGTH) {
            token[MAX_WORD_LENGTH - 1] = '\0';
            token_len = MAX_WORD_LENGTH - 1;
        }
        
        char* lower_token = (char*)malloc(token_len + 1);
        if (lower_token == NULL) {
            free(str_copy);
            for (int i = 0; i < word_count; i++) {
                free(words[i]);
            }
            return NULL;
        }
        
        for (size_t i = 0; i < token_len; i++) {
            lower_token[i] = tolower((unsigned char)token[i]);
        }
        lower_token[token_len] = '\0';
        
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], lower_token) == 0) {
                char* result = strdup(token);
                free(lower_token);
                free(str_copy);
                for (int j = 0; j < word_count; j++) {
                    free(words[j]);
                }
                return result;
            }
        }
        
        words[word_count] = lower_token;
        word_count++;
        token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}");
    }
    
    free(str_copy);
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    
    return NULL;
}

int main(void) {
    char* input = (char*)malloc(MAX_INPUT_LENGTH);
    if (input == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    
    printf("Enter a string: ");
    if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
        printf("Error reading input.\n");
        free(input);
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    char* result = find_first_repeated_word(input);
    
    if (result != NULL) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated word found.\n");
    }
    
    free(input);
    
    return 0;
}