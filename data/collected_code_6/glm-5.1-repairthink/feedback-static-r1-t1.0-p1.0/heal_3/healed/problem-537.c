#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

char* find_first_repeated_word(char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strnlen(str, SIZE_MAX);
    if (len == SIZE_MAX) {
        return NULL;
    }

    char *buffer = (char *)malloc(len + 1);
    if (buffer == NULL) {
        return NULL;
    }
    
    snprintf(buffer, len + 1, "%s", str);

    char **words = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    char *token = strtok(buffer, " \t\n\r.,;:!?()-");
    
    while (token != NULL) {
        if (word_count == capacity) {
            if (capacity > SIZE_MAX / sizeof(char *) / 2) {
                free(buffer);
                free(words);
                return NULL;
            }
            capacity = capacity == 0 ? 16 : capacity * 2;
            char **temp = (char **)realloc(words, capacity * sizeof(char *));
            if (temp == NULL) {
                free(buffer);
                free(words);
                return NULL;
            }
            words = temp;
        }
        
        size_t token_len = 0;
        while (token[token_len] != '\0') {
            token[token_len] = tolower((unsigned char)token[token_len]);
            token_len++;
        }
        
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                char *result = (char *)malloc(token_len + 1);
                if (result == NULL) {
                    free(buffer);
                    free(words);
                    return NULL;
                }
                snprintf(result, token_len + 1, "%s", token);
                free(buffer);
                free(words);
                return result;
            }
        }
        
        words[word_count++] = token;
        token = strtok(NULL, " \t\n\r.,;:!?()-");
    }
    
    free(buffer);
    free(words);
    return NULL;
}

int main() {
    char str[] = "Hello world! This is a test. Hello again.";
    char *repeated = find_first_repeated_word(str);
    
    if (repeated != NULL) {
        printf("%s\n", repeated);
        free(repeated);
    } else {
        printf("NULL\n");
    }
    
    return 0;
}