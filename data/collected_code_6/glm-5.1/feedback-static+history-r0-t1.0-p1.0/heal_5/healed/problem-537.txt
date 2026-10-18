#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

    if (memcpy(buffer, str, len + 1) == NULL) {
        free(buffer);
        return NULL;
    }

    char **words = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    char *token = strtok(buffer, " \t\n\r.,;:!?()-");
    
    while (token != NULL) {
        if (word_count == capacity) {
            capacity = capacity == 0 ? 16 : capacity * 2;
            char **temp = (char **)realloc(words, capacity * sizeof(char *));
            if (temp == NULL) {
                free(buffer);
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            words = temp;
        }
        
        size_t token_len = strnlen(token, SIZE_MAX);
        if (token_len == SIZE_MAX) {
            free(buffer);
            for (size_t i = 0; i < word_count; i++) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }

        char *lower_token = (char *)malloc(token_len + 1);
        if (lower_token == NULL) {
            free(buffer);
            for (size_t i = 0; i < word_count; i++) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }
        
        for (size_t i = 0; token[i] != '\0'; i++) {
            lower_token[i] = tolower((unsigned char)token[i]);
        }
        lower_token[token_len] = '\0';
        
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], lower_token) == 0) {
                char *result = (char *)malloc(token_len + 1);
                if (result == NULL) {
                    free(lower_token);
                    free(buffer);
                    for (size_t j = 0; j < word_count; j++) {
                        free(words[j]);
                    }
                    free(words);
                    return NULL;
                }
                
                if (memcpy(result, lower_token, token_len + 1) == NULL) {
                    free(result);
                    free(lower_token);
                    free(buffer);
                    for (size_t j = 0; j < word_count; j++) {
                        free(words[j]);
                    }
                    free(words);
                    return NULL;
                }
                
                free(lower_token);
                free(buffer);
                for (size_t j = 0; j < word_count; j++) {
                    free(words[j]);
                }
                free(words);
                return result;
            }
        }
        
        words[word_count++] = lower_token;
        token = strtok(NULL, " \t\n\r.,;:!?()-");
    }
    
    free(buffer);
    for (size_t i = 0; i < word_count; i++) {
        free(words[i]);
    }
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