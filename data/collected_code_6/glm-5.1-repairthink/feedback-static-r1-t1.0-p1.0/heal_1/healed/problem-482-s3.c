#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <stdint.h>

int find_sequences(const char *str, char ***results, size_t *count) {
    regex_t regex;
    regmatch_t match;
    int reti;
    size_t capacity = 10;
    size_t idx = 0;
    char **arr = NULL;

    reti = regcomp(&regex, "[A-Z][a-z]+", REG_EXTENDED);
    if (reti != 0) {
        return -1;
    }

    if (capacity > SIZE_MAX / sizeof(char *)) {
        regfree(&regex);
        return -1;
    }

    arr = malloc(capacity * sizeof(char *));
    if (arr == NULL) {
        regfree(&regex);
        return -1;
    }

    const char *p = str;
    while (1) {
        reti = regexec(&regex, p, 1, &match, 0);
        if (reti == REG_NOMATCH) {
            break;
        }
        if (reti != 0) {
            for (size_t i = 0; i < idx; i++) free(arr[i]);
            free(arr);
            regfree(&regex);
            return -1;
        }

        if (idx >= capacity) {
            if (capacity > SIZE_MAX / 2) {
                for (size_t i = 0; i < idx; i++) free(arr[i]);
                free(arr);
                regfree(&regex);
                return -1;
            }
            capacity *= 2;
            if (capacity > SIZE_MAX / sizeof(char *)) {
                for (size_t i = 0; i < idx; i++) free(arr[i]);
                free(arr);
                regfree(&regex);
                return -1;
            }
            char **tmp = realloc(arr, capacity * sizeof(char *));
            if (tmp == NULL) {
                for (size_t i = 0; i < idx; i++) free(arr[i]);
                free(arr);
                regfree(&regex);
                return -1;
            }
            arr = tmp;
        }

        if (match.rm_so < 0 || match.rm_eo < 0 || match.rm_eo < match.rm_so) {
            for (size_t i = 0; i < idx; i++) free(arr[i]);
            free(arr);
            regfree(&regex);
            return -1;
        }

        size_t len = (size_t)(match.rm_eo - match.rm_so);

        if (len == SIZE_MAX) {
            for (size_t i = 0; i < idx; i++) free(arr[i]);
            free(arr);
            regfree(&regex);
            return -1;
        }

        arr[idx] = malloc(len + 1);
        if (arr[idx] == NULL) {
            for (size_t i = 0; i < idx; i++) free(arr[i]);
            free(arr);
            regfree(&regex);
            return -1;
        }

        memcpy(arr[idx], p + match.rm_so, len);
        arr[idx][len] = '\0';
        idx++;

        p += match.rm_eo;
    }

    regfree(&regex);

    if (idx == 0) {
        free(arr);
        *results = NULL;
        *count = 0;
        return 0;
    }

    char **shrunk = realloc(arr, idx * sizeof(char *));
    if (shrunk != NULL) {
        arr = shrunk;
    }

    *results = arr;
    *count = idx;
    return 0;
}

void free_results(char **results, size_t count) {
    if (results == NULL) return;
    for (size_t i = 0; i < count; i++) {
        free(results[i]);
    }
    free(results);
}

int main(void) {
    const char *input = "Hello World from The C Programming Language";
    char **results = NULL;
    size_t count = 0;

    int rc = find_sequences(input, &results, &count);
    if (rc == 0) {
        for (size_t i = 0; i < count; i++) {
            printf("%s\n", results[i]);
        }
        free_results(results, count);
    } else {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}