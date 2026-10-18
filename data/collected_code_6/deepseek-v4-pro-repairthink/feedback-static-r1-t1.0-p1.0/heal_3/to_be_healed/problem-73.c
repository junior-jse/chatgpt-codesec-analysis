#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stddef.h>

char **split_regex(const char *str, size_t len, const char *pattern, size_t *count) {
    regex_t regex;
    regmatch_t match;
    char **result = NULL;
    size_t capacity = 10;
    size_t size = 0;
    const char *cursor = str;
    const char *end = str + len;

    if (regcomp(&regex, pattern, REG_EXTENDED)) {
        *count = 0;
        return NULL;
    }

    result = malloc(capacity * sizeof(char *));
    if (!result) {
        regfree(&regex);
        *count = 0;
        return NULL;
    }

    while (cursor <= end && regexec(&regex, cursor, 1, &match, 0) == 0) {
        const char *token_start = cursor;
        size_t token_len = (size_t)match.rm_so;

        if (token_len >= SIZE_MAX - 1) {
            for (size_t i = 0; i < size; i++) free(result[i]);
            free(result);
            regfree(&regex);
            *count = 0;
            return NULL;
        }

        if (size == capacity) {
            capacity *= 2;
            char **temp = realloc(result, capacity * sizeof(char *));
            if (!temp) {
                for (size_t i = 0; i < size; i++) free(result[i]);
                free(result);
                regfree(&regex);
                *count = 0;
                return NULL;
            }
            result = temp;
        }

        result[size] = malloc(token_len + 1);
        if (!result[size]) {
            for (size_t i = 0; i < size; i++) free(result[i]);
            free(result);
            regfree(&regex);
            *count = 0;
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[size], token_start, token_len);
        result[size][token_len] = '\0';
        size++;

        cursor += match.rm_so + match.rm_eo;

        if (match.rm_so == 0 && match.rm_eo == 0) {
            cursor++;
        }
    }

    if (cursor < end) {
        size_t remaining_len = (size_t)(end - cursor);

        if (remaining_len >= SIZE_MAX - 1) {
            for (size_t i = 0; i < size; i++) free(result[i]);
            free(result);
            regfree(&regex);
            *count = 0;
            return NULL;
        }

        if (size == capacity) {
            capacity *= 2;
            char **temp = realloc(result, capacity * sizeof(char *));
            if (!temp) {
                for (size_t i = 0; i < size; i++) free(result[i]);
                free(result);
                regfree(&regex);
                *count = 0;
                return NULL;
            }
            result = temp;
        }

        result[size] = malloc(remaining_len + 1);
        if (!result[size]) {
            for (size_t i = 0; i < size; i++) free(result[i]);
            free(result);
            regfree(&regex);
            *count = 0;
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[size], cursor, remaining_len);
        result[size][remaining_len] = '\0';
        size++;
    }

    regfree(&regex);
    *count = size;
    return result;
}

void free_split_result(char **result, size_t count) {
    if (result) {
        for (size_t i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
    }
}

int main(void) {
    const char *text = "apple,banana;cherry:date|elderberry";
    const char *pattern = "[,;:|]";
    size_t count = 0;

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    char **tokens = split_regex(text, strlen(text), pattern, &count);

    if (tokens) {
        printf("Split results (%zu tokens):\n", count);
        for (size_t i = 0; i < count; i++) {
            printf("Token %zu: %s\n", i, tokens[i]);
        }
        free_split_result(tokens, count);
    } else {
        printf("Failed to split string\n");
    }

    return 0;
}