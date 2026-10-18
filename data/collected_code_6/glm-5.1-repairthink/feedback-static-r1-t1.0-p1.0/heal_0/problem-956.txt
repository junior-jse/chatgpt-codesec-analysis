#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char **split_at_uppercase(const char *str, size_t *count) {
    regex_t regex;
    regmatch_t match;
    int reti;
    size_t capacity = 10;
    size_t idx = 0;
    char **result = malloc(capacity * sizeof(char *));
    const char *cursor = str;

    if (!result) {
        *count = 0;
        return NULL;
    }

    reti = regcomp(&regex, "[A-Z]", REG_EXTENDED);
    if (reti) {
        free(result);
        *count = 0;
        return NULL;
    }

    while ((reti = regexec(&regex, cursor, 1, &match, 0)) == 0) {
        if (idx >= capacity) {
            capacity *= 2;
            char **temp = realloc(result, capacity * sizeof(char *));
            if (!temp) {
                for (size_t i = 0; i < idx; i++) free(result[i]);
                free(result);
                regfree(&regex);
                *count = 0;
                return NULL;
            }
            result = temp;
        }

        size_t len = match.rm_so;
        result[idx] = malloc(len + 1);
        if (!result[idx]) {
            for (size_t i = 0; i < idx; i++) free(result[i]);
            free(result);
            regfree(&regex);
            *count = 0;
            return NULL;
        }
        memcpy(result[idx], cursor, len);
        result[idx][len] = '\0';
        idx++;

        cursor += match.rm_so;
        len = match.rm_eo - match.rm_so;
        result[idx] = malloc(len + 1);
        if (!result[idx]) {
            for (size_t i = 0; i < idx; i++) free(result[i]);
            free(result);
            regfree(&regex);
            *count = 0;
            return NULL;
        }
        memcpy(result[idx], cursor, len);
        result[idx][len] = '\0';
        idx++;

        cursor += len;
    }

    if (*cursor != '\0') {
        if (idx >= capacity) {
            capacity += 1;
            char **temp = realloc(result, capacity * sizeof(char *));
            if (!temp) {
                for (size_t i = 0; i < idx; i++) free(result[i]);
                free(result);
                regfree(&regex);
                *count = 0;
                return NULL;
            }
            result = temp;
        }

        size_t len = strlen(cursor);
        result[idx] = malloc(len + 1);
        if (!result[idx]) {
            for (size_t i = 0; i < idx; i++) free(result[i]);
            free(result);
            regfree(&regex);
            *count = 0;
            return NULL;
        }
        memcpy(result[idx], cursor, len + 1);
        idx++;
    }

    regfree(&regex);

    char **final_result = realloc(result, (idx + 1) * sizeof(char *));
    if (!final_result) {
        for (size_t i = 0; i < idx; i++) free(result[i]);
        free(result);
        *count = 0;
        return NULL;
    }
    result = final_result;
    result[idx] = NULL;

    *count = idx;
    return result;
}

void free_split_result(char **result) {
    if (!result) return;
    for (size_t i = 0; result[i] != NULL; i++) {
        free(result[i]);
    }
    free(result);
}

int main(void) {
    const char *input = "SplitThisStringAtUppercaseLetters";
    size_t count = 0;
    char **parts = split_at_uppercase(input, &count);

    if (parts) {
        for (size_t i = 0; i < count; i++) {
            printf("%s\n", parts[i]);
        }
        free_split_result(parts);
    }

    return 0;
}