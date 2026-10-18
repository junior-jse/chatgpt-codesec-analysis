#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void free_tokens(char **tokens, size_t count)
{
    if (tokens == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(tokens[i]);
    }

    free(tokens);
}

static int add_token(char ***tokens, size_t *count, size_t *capacity,
                     const char *start, size_t length)
{
    if (*count == *capacity) {
        size_t new_capacity = (*capacity == 0U) ? 8U : *capacity * 2U;

        if (new_capacity < *capacity ||
            new_capacity > SIZE_MAX / sizeof(**tokens)) {
            return -1;
        }

        char **resized = realloc(*tokens, new_capacity * sizeof(**tokens));
        if (resized == NULL) {
            return -1;
        }

        *tokens = resized;
        *capacity = new_capacity;
    }

    char *token = malloc(length + 1U);
    if (token == NULL) {
        return -1;
    }

    memcpy(token, start, length);
    token[length] = '\0';
    (*tokens)[*count] = token;
    ++(*count);

    return 0;
}

static int split_regex(const char *input, const char *delimiter_pattern,
                       char ***result, size_t *result_count)
{
    regex_t regex;
    char **tokens = NULL;
    size_t count = 0U;
    size_t capacity = 0U;
    size_t offset = 0U;
    int status;

    if (input == NULL || delimiter_pattern == NULL ||
        result == NULL || result_count == NULL) {
        return -1;
    }

    *result = NULL;
    *result_count = 0U;

    status = regcomp(&regex, delimiter_pattern, REG_EXTENDED);
    if (status != 0) {
        return -1;
    }

    while (input[offset] != '\0') {
        regmatch_t match;
        status = regexec(&regex, input + offset, 1U, &match, 0);

        if (status == REG_NOMATCH) {
            if (add_token(&tokens, &count, &capacity, input + offset,
                          strlen(input + offset)) != 0) {
                free_tokens(tokens, count);
                regfree(&regex);
                return -1;
            }
            offset += strlen(input + offset);
            break;
        }

        if (status != 0 || match.rm_so < 0 || match.rm_eo <= match.rm_so) {
            free_tokens(tokens, count);
            regfree(&regex);
            return -1;
        }

        if (match.rm_so > 0 &&
            add_token(&tokens, &count, &capacity, input + offset,
                      (size_t)match.rm_so) != 0) {
            free_tokens(tokens, count);
            regfree(&regex);
            return -1;
        }

        offset += (size_t)match.rm_eo;
    }

    regfree(&regex);
    *result = tokens;
    *result_count = count;

    return 0;
}

int main(void)
{
    const char *input = "alpha,beta;gamma|delta  epsilon";
    const char *delimiters = "[,;|[:space:]]+";
    char **tokens = NULL;
    size_t count = 0U;

    if (split_regex(input, delimiters, &tokens, &count) != 0) {
        fputs("Failed to split input\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < count; ++i) {
        if (printf("%s\n", tokens[i]) < 0) {
            free_tokens(tokens, count);
            return EXIT_FAILURE;
        }
    }

    free_tokens(tokens, count);
    return EXIT_SUCCESS;
}