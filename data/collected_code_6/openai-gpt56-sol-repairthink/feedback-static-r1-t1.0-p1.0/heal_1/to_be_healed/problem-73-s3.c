#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void free_tokens(char **tokens, size_t count)
{
    size_t i;

    if (tokens == NULL) {
        return;
    }

    for (i = 0; i < count; ++i) {
        free(tokens[i]);
    }

    free(tokens);
}

static int append_token(char ***tokens, size_t *count, size_t *capacity,
                        const char *start, size_t length)
{
    char **resized;
    char *token;
    /* Possible weaknesses found:
     *  The scope of the variable 'new_capacity' can be reduced. [variableScope]
     */
    size_t new_capacity;

    if (tokens == NULL || count == NULL || capacity == NULL ||
        (start == NULL && length != 0)) {
        return EINVAL;
    }

    if (length == SIZE_MAX) {
        return EOVERFLOW;
    }

    token = malloc(length + 1);
    if (token == NULL) {
        return ENOMEM;
    }

    if (length != 0) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(token, start, length);
    }
    token[length] = '\0';

    if (*count == *capacity) {
        if (*capacity > SIZE_MAX / 2) {
            free(token);
            return EOVERFLOW;
        }

        new_capacity = *capacity == 0 ? 8 : *capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(**tokens)) {
            free(token);
            return EOVERFLOW;
        }

        resized = realloc(*tokens, new_capacity * sizeof(**tokens));
        if (resized == NULL) {
            free(token);
            return ENOMEM;
        }

        *tokens = resized;
        *capacity = new_capacity;
    }

    (*tokens)[*count] = token;
    ++(*count);
    return 0;
}

static int split_regex(const char *input, const char *delimiter_pattern,
                       char ***result, size_t *result_count)
{
    regex_t regex;
    regmatch_t match;
    char **tokens = NULL;
    size_t count = 0;
    size_t capacity = 0;
    size_t offset = 0;
    size_t input_length;
    int status;
    int regex_status;

    if (input == NULL || delimiter_pattern == NULL ||
        result == NULL || result_count == NULL) {
        return EINVAL;
    }

    *result = NULL;
    *result_count = 0;

    regex_status = regcomp(&regex, delimiter_pattern, REG_EXTENDED);
    if (regex_status != 0) {
        return EINVAL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    input_length = strlen(input);

    while (offset <= input_length) {
        regex_status = regexec(&regex, input + offset, 1, &match, 0);
        if (regex_status == REG_NOMATCH) {
            status = append_token(&tokens, &count, &capacity,
                                  input + offset, input_length - offset);
            if (status != 0) {
                regfree(&regex);
                free_tokens(tokens, count);
                return status;
            }
            break;
        }

        if (regex_status != 0 || match.rm_so < 0 || match.rm_eo < 0 ||
            match.rm_eo < match.rm_so || match.rm_so == match.rm_eo) {
            regfree(&regex);
            free_tokens(tokens, count);
            return EINVAL;
        }

        status = append_token(&tokens, &count, &capacity, input + offset,
                              (size_t)match.rm_so);
        if (status != 0) {
            regfree(&regex);
            free_tokens(tokens, count);
            return status;
        }

        if ((size_t)match.rm_eo > input_length - offset) {
            regfree(&regex);
            free_tokens(tokens, count);
            return EINVAL;
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
    const char *input = "alpha,beta;gamma delta|epsilon";
    const char *delimiters = "[,;|[:space:]]+";
    char **tokens = NULL;
    size_t count = 0;
    size_t i;
    int status;

    status = split_regex(input, delimiters, &tokens, &count);
    if (status != 0) {
        fprintf(stderr, "Failed to split string: %s\n", strerror(status));
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        if (printf("%s\n", tokens[i]) < 0) {
            free_tokens(tokens, count);
            return EXIT_FAILURE;
        }
    }

    free_tokens(tokens, count);
    return EXIT_SUCCESS;
}