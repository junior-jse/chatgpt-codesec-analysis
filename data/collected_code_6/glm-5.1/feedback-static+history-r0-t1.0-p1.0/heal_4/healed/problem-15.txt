#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** split_at_lowercase(const char* str, size_t* count) {
    if (!str) {
        *count = 0;
        return NULL;
    }

    size_t capacity = 16;
    size_t num_tokens = 0;
    
    char** result = malloc(capacity * sizeof(char*));
    if (!result) {
        return NULL;
    }
    
    const char* start = str;
    
    while (*start) {
        while (*start && islower((unsigned char)*start)) {
            start++;
        }
        if (!*start) {
            break;
        }
        
        const char* end = start;
        while (*end && !islower((unsigned char)*end)) {
            end++;
        }
        
        size_t token_len = end - start;
        char* token = malloc(token_len + 1);
        if (!token) {
            for (size_t i = 0; i < num_tokens; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        if (token_len > 0) {
            memcpy(token, start, token_len);
        }
        token[token_len] = '\0';
        
        if (num_tokens == capacity) {
            size_t new_capacity = capacity * 2;
            if (new_capacity <= capacity) {
                free(token);
                for (size_t i = 0; i < num_tokens; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            capacity = new_capacity;
            char** new_result = realloc(result, capacity * sizeof(char*));
            if (!new_result) {
                free(token);
                for (size_t i = 0; i < num_tokens; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = new_result;
        }
        
        result[num_tokens++] = token;
        start = end;
    }
    
    char** final_result = realloc(result, (num_tokens + 1) * sizeof(char*));
    if (!final_result) {
        for (size_t i = 0; i < num_tokens; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    result = final_result;
    result[num_tokens] = NULL;
    
    *count = num_tokens;
    return result;
}

int main(void) {
    const char* input = "ABCdefGHIjklMNO";
    size_t count = 0;
    
    char** tokens = split_at_lowercase(input, &count);
    if (!tokens) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    for (size_t i = 0; i < count; i++) {
        printf("%s\n", tokens[i]);
        free(tokens[i]);
    }
    free(tokens);
    
    return 0;
}