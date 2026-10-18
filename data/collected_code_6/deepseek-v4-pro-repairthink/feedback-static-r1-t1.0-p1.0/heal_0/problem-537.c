#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000

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
        char* lower_token = strdup(token);
        if (lower_token == NULL) {
            free(str_copy);
            for (int i = 0; i < word_count; i++) {
                free(words[i]);
            }
            return NULL;
        }
        
        for (int i = 0; lower_token[i]; i++) {
            lower_token[i] = tolower(lower_token[i]);
        }
        
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

int main() {
    char input[1000];
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    char* result = find_first_repeated_word(input);
    
    if (result != NULL) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated word found.\n");
    }
    
    return 0;
}