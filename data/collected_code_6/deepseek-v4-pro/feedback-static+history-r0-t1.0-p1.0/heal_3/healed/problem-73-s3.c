#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

static char** split_string_internal(const char* input, size_t input_len, 
                                     const char* pattern, int* count) {
    regex_t regex;
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    char** result = NULL;
    int capacity = 0;
    *count = 0;

    const char* current = input;
    const char* search_start = input;
    regmatch_t match;

    while (1) {
        int match_result = regexec(&regex, search_start, 1, &match, 0);
        
        if (match_result == REG_NOMATCH) {
            size_t remaining_len = input_len - (current - input);
            if (remaining_len > 0) {
                if (*count >= capacity) {
                    capacity = capacity == 0 ? 4 : capacity * 2;
                    if (capacity > SIZE_MAX / sizeof(char*)) {
                        for (int i = 0; i < *count; i++) {
                            free(result[i]);
                        }
                        free(result);
                        regfree(&regex);
                        return NULL;
                    }
                    char** new_result = realloc(result, capacity * sizeof(char*));
                    if (new_result == NULL) {
                        for (int i = 0; i < *count; i++) {
                            free(result[i]);
                        }
                        free(result);
                        regfree(&regex);
                        return NULL;
                    }
                    result = new_result;
                }
                if (remaining_len > SIZE_MAX - 1) {
                    for (int i = 0; i < *count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                result[*count] = malloc(remaining_len + 1);
                if (result[*count] == NULL) {
                    for (int i = 0; i < *count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                if (remaining_len > 0) {
                    memcpy(result[*count], current, remaining_len);
                }
                result[*count][remaining_len] = '\0';
                (*count)++;
            }
            break;
        } else if (match_result != 0) {
            for (int i = 0; i < *count; i++) {
                free(result[i]);
            }
            free(result);
            regfree(&regex);
            return NULL;
        }

        size_t token_len = (search_start - current) + match.rm_so;
        if (token_len > 0) {
            if (*count >= capacity) {
                capacity = capacity == 0 ? 4 : capacity * 2;
                if (capacity > SIZE_MAX / sizeof(char*)) {
                    for (int i = 0; i < *count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                char** new_result = realloc(result, capacity * sizeof(char*));
                if (new_result == NULL) {
                    for (int i = 0; i < *count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    regfree(&regex);
                    return NULL;
                }
                result = new_result;
            }
            if (token_len > SIZE_MAX - 1) {
                for (int i = 0; i < *count; i++) {
                    free(result[i]);
                }
                free(result);
                regfree(&regex);
                return NULL;
            }
            char* token = malloc(token_len + 1);
            if (token == NULL) {
                for (int i = 0; i < *count; i++) {
                    free(result[i]);
                }
                free(result);
                regfree(&regex);
                return NULL;
            }
            if (token_len > 0) {
                memcpy(token, current, token_len);
            }
            token[token_len] = '\0';
            result[*count] = token;
            (*count)++;
        }

        current = search_start + match.rm_eo;
        search_start = current;

        if ((size_t)(current - input) >= input_len) {
            break;
        }
    }

    regfree(&regex);
    return result;
}

char** split_string(const char* input, const char* pattern, int* count) {
    if (input == NULL || pattern == NULL || count == NULL) {
        return NULL;
    }
    
    size_t input_len = strnlen(input, SIZE_MAX);
    if (input_len == SIZE_MAX && input[0] != '\0') {
        return NULL;
    }
    
    return split_string_internal(input, input_len, pattern, count);
}

void free_split_result(char** result, int count) {
    if (result == NULL) {
        return;
    }
    for (int i = 0; i < count; i++) {
        free(result[i]);
    }
    free(result);
}

int main() {
    const char* input = "Hello,world;this:is|a test";
    const char* pattern = "[,;:| ]+";
    int count = 0;
    
    char** tokens = split_string(input, pattern, &count);
    
    if (tokens == NULL) {
        printf("Failed to split string\n");
        return 1;
    }
    
    printf("Split into %d tokens:\n", count);
    for (int i = 0; i < count; i++) {
        printf("Token %d: '%s'\n", i, tokens[i]);
    }
    
    free_split_result(tokens, count);
    return 0;
}