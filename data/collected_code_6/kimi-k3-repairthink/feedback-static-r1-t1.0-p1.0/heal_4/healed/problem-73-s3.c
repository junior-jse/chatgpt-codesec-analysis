#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <regex.h>

#define MAX_INPUT_LENGTH ((size_t)16 * 1024 * 1024)
#define INITIAL_CAPACITY 10

static void cleanup_tokens(char **tokens, size_t count) {
    size_t i;

    if (tokens == NULL) {
        return;
    }

    for (i = 0; i < count; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

static int grow_tokens(char ***tokens, size_t *capacity, size_t needed) {
    size_t new_capacity = *capacity;
    char **temp;

    while (new_capacity < needed) {
        if (new_capacity > SIZE_MAX / 2) {
            return -1;
        }
        new_capacity *= 2;
    }

    if (new_capacity > SIZE_MAX / sizeof(char *)) {
        return -1;
    }

    temp = realloc(*tokens, new_capacity * sizeof(char *));
    if (temp == NULL) {
        return -1;
    }

    *tokens = temp;
    *capacity = new_capacity;
    return 0;
}

static int append_token(char ***tokens, size_t *token_count, size_t *capacity,
                        const char *src, size_t len) {
    size_t dest_size;
    char *dest;
    size_t i;

    if (len >= SIZE_MAX) {
        return -1;
    }
    dest_size = len + 1;

    if (*token_count >= *capacity) {
        if (grow_tokens(tokens, capacity, *token_count + 1) != 0) {
            return -1;
        }
    }

    dest = malloc(dest_size);
    if (dest == NULL) {
        return -1;
    }

    for (i = 0; i < len; i++) {
        dest[i] = src[i];
    }
    dest[len] = '\0';

    (*tokens)[*token_count] = dest;
    (*token_count)++;

    return 0;
}

char **split_string_regex(const char *input, const char *pattern, size_t *count) {
    regex_t regex;
    regmatch_t match;
    char **tokens = NULL;
    size_t token_count = 0;
    size_t capacity = INITIAL_CAPACITY;
    const char *cursor;
    size_t remaining;
    int ret;

    if (input == NULL || pattern == NULL || count == NULL) {
        return NULL;
    }

    *count = 0;

    remaining = strnlen(input, MAX_INPUT_LENGTH);
    if (remaining == MAX_INPUT_LENGTH) {
        return NULL;
    }

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    tokens = malloc(capacity * sizeof(char *));
    if (tokens == NULL) {
        regfree(&regex);
        return NULL;
    }

    cursor = input;

    while (remaining > 0) {
        ret = regexec(&regex, cursor, 1, &match, 0);

        if (ret == REG_NOMATCH) {
            if (append_token(&tokens, &token_count, &capacity, cursor, remaining) != 0) {
                cleanup_tokens(tokens, token_count);
                regfree(&regex);
                return NULL;
            }
            break;
        } else if (ret != 0) {
            cleanup_tokens(tokens, token_count);
            regfree(&regex);
            return NULL;
        }

        if (match.rm_so > 0) {
            if (append_token(&tokens, &token_count, &capacity, cursor,
                             (size_t)match.rm_so) != 0) {
                cleanup_tokens(tokens, token_count);
                regfree(&regex);
                return NULL;
            }
        }

        if (match.rm_eo > 0) {
            cursor += match.rm_eo;
            remaining -= (size_t)match.rm_eo;
        } else {
            cursor++;
            remaining--;
        }
    }

    if (token_count >= capacity) {
        if (grow_tokens(&tokens, &capacity, token_count + 1) != 0) {
            cleanup_tokens(tokens, token_count);
            regfree(&regex);
            return NULL;
        }
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
    size_t i;
    char **tokens;

    tokens = split_string_regex(input, pattern, &count);

    if (tokens == NULL) {
        fprintf(stderr, "Error splitting string\n");
        return EXIT_FAILURE;
    }

    printf("Tokens found: %zu\n", count);
    for (i = 0; i < count; i++) {
        printf("Token %zu: %s\n", i, tokens[i]);
    }

    free_tokens(tokens, count);
    return EXIT_SUCCESS;
}