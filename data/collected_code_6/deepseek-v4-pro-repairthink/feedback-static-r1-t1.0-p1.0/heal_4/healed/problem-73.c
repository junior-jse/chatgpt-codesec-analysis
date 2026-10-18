#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stddef.h>
#include <stdint.h>

static inline void safe_copy(char *dst, size_t dstsize, const char *src, size_t len) {
    if (len >= dstsize) {
        if (dstsize > 0) {
            memcpy(dst, src, dstsize - 1);
            dst[dstsize - 1] = '\0';
        }
    } else {
        memcpy(dst, src, len);
        dst[len] = '\0';
    }
}

size_t safe_strlen(const char *s, size_t maxlen) {
    size_t i;
    for (i = 0; i < maxlen && s[i] != '\0'; i++) {}
    return i;
}

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

        safe_copy(result[size], token_len + 1, token_start, token_len);
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

        safe_copy(result[size], remaining_len + 1, cursor, remaining_len);
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
    size_t len = safe_strlen(text, SIZE_MAX);

    char **tokens = split_regex(text, len, pattern, &count);

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