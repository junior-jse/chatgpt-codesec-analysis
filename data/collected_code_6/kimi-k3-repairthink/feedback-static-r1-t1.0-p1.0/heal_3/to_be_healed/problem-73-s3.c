#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

static void cleanup_tokens(char **tokens, size_t count) {
    if (tokens == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

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
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t len = strlen(cursor);
            /* Possible weaknesses found:
             *  Assignment 'dest_size=len+1', assigned value is greater than 0
             */
            size_t dest_size = len + 1;

            /* Possible weaknesses found:
             *  Condition 'dest_size==0' is always false
             *  Condition 'dest_size==0' is always false [knownConditionTrueFalse]
             */
            if (dest_size == 0) {
                cleanup_tokens(tokens, token_count);
                regfree(&regex);
                return NULL;
            }

            if (token_count >= capacity) {
                capacity *= 2;
                char **temp = realloc(tokens, capacity * sizeof(char *));
                if (temp == NULL) {
                    cleanup_tokens(tokens, token_count);
                    regfree(&regex);
                    return NULL;
                }
                tokens = temp;
            }

            tokens[token_count] = malloc(dest_size);
            if (tokens[token_count] == NULL) {
                cleanup_tokens(tokens, token_count);
                regfree(&regex);
                return NULL;
            }

            if (len < dest_size) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(tokens[token_count], cursor, len);
                tokens[token_count][len] = '\0';
            } else {
                free(tokens[token_count]);
                cleanup_tokens(tokens, token_count);
                regfree(&regex);
                return NULL;
            }
            token_count++;
            break;
        } else if (ret != 0) {
            cleanup_tokens(tokens, token_count);
            regfree(&regex);
            return NULL;
        }

        if (match.rm_so > 0) {
            if (token_count >= capacity) {
                capacity *= 2;
                char **temp = realloc(tokens, capacity * sizeof(char *));
                if (temp == NULL) {
                    cleanup_tokens(tokens, token_count);
                    regfree(&regex);
                    return NULL;
                }
                tokens = temp;
            }

            size_t len = (size_t)match.rm_so;
            /* Possible weaknesses found:
             *  Assignment 'dest_size=len+1', assigned value is greater than 0
             */
            size_t dest_size = len + 1;

            /* Possible weaknesses found:
             *  Condition 'dest_size==0' is always false
             *  Condition 'dest_size==0' is always false [knownConditionTrueFalse]
             */
            if (dest_size == 0) {
                cleanup_tokens(tokens, token_count);
                regfree(&regex);
                return NULL;
            }

            tokens[token_count] = malloc(dest_size);
            if (tokens[token_count] == NULL) {
                cleanup_tokens(tokens, token_count);
                regfree(&regex);
                return NULL;
            }

            if (len < dest_size) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(tokens[token_count], cursor, len);
                tokens[token_count][len] = '\0';
            } else {
                free(tokens[token_count]);
                cleanup_tokens(tokens, token_count);
                regfree(&regex);
                return NULL;
            }
            token_count++;
        }

        if (match.rm_eo > 0) {
            cursor += match.rm_eo;
        } else {
            cursor++;
        }
    }

    if (token_count >= capacity) {
        capacity++;
        char **temp = realloc(tokens, capacity * sizeof(char *));
        if (temp == NULL) {
            cleanup_tokens(tokens, token_count);
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
    cleanup_tokens(tokens, count);
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