#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char **split_string_regex(const char *input, const char *pattern, size_t *count) {
    regex_t regex;
    regmatch_t match;
    char **tokens = NULL;
    size_t token_count = 0;
    size_t capacity = 10;
    const char *cursor = input;
    int ret;

    if (input == NULL || pattern == NULL || count == NULL) {
        return NULL;
    }

    *count = 0;

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    tokens = malloc(capacity * sizeof(char *));
    if (tokens == NULL) {
        regfree(&regex);
        return NULL;
    }

    while (*cursor != '\0') {
        ret = regexec(&regex, cursor, 1, &match, 0);
        
        if (ret == REG_NOMATCH) {
            if (token_count >= capacity) {
                capacity *= 2;
                char **temp = realloc(tokens, capacity * sizeof(char *));
                if (temp == NULL) {
                    for (size_t i = 0; i < token_count; i++) {
                        free(tokens[i]);
                    }
                    free(tokens);
                    regfree(&regex);
                    return NULL;
                }
                tokens = temp;
            }
            
            tokens[token_count] = strdup(cursor);
            if (tokens[token_count] == NULL) {
                for (size_t i = 0; i < token_count; i++) {
                    free(tokens[i]);
                }
                free(tokens);
                regfree(&regex);
                return NULL;
            }
            token_count++;
            break;
        } else if (ret != 0) {
            for (size_t i = 0; i < token_count; i++) {
                free(tokens[i]);
            }
            free(tokens);
            regfree(&regex);
            return NULL;
        }

        if (match.rm_so > 0) {
            if (token_count >= capacity) {
                capacity *= 2;
                char **temp = realloc(tokens, capacity * sizeof(char *));
                if (temp == NULL) {
                    for (size_t i = 0; i < token_count; i++) {
                        free(tokens[i]);
                    }
                    free(tokens);
                    regfree(&regex);
                    return NULL;
                }
                tokens = temp;
            }

            size_t len = match.rm_so;
            tokens[token_count] = malloc(len + 1);
            if (tokens[token_count] == NULL) {
                for (size_t i = 0; i < token_count; i++) {
                    free(tokens[i]);
                }
                free(tokens);
                regfree(&regex);
                return NULL;
            }
            strncpy(tokens[token_count], cursor, len);
            tokens[token_count][len] = '\0';
            token_count++;
        }

        cursor += match.rm_eo;
    }

    if (token_count >= capacity) {
        capacity++;
        char **temp = realloc(tokens, capacity * sizeof(char *));
        if (temp == NULL) {
            for (size_t i = 0; i < token_count; i++) {
                free(tokens[i]);
            }
            free(tokens);
            regfree(&regex);
            return NULL;
        }
        tokens = temp;
    }
    tokens[token_count] = NULL;

    *count = token_count;
    regfree(&regex);
    return tokens;
}

void free_tokens(char **tokens, size_t count) {
    if (tokens == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

int main(void) {
    const char *input = "apple,banana;cherry orange|grape";
    const char *pattern = "[,; |]+";
    size_t count = 0;

    char **tokens = split_string_regex(input, pattern, &count);

    if (tokens == NULL) {
        fprintf(stderr, "Error splitting string\n");
        return EXIT_FAILURE;
    }

    printf("Tokens found: %zu\n", count);
    for (size_t i = 0; i < count; i++) {
        printf("Token %zu: %s\n", i, tokens[i]);
    }

    free_tokens(tokens, count);
    return EXIT_SUCCESS;
}