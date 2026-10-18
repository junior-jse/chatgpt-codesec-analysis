#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <limits.h>
#include <stdint.h>

char **split_string_regex(const char *str, const char *pattern, size_t *count) {
    regex_t regex;
    regmatch_t match;
    int ret;
    size_t capacity = 10;
    size_t n = 0;
    char **result = NULL;
    size_t str_len;
    size_t current = 0;
    int regex_compiled = 0;

    if (str == NULL || pattern == NULL || count == NULL) {
        goto fail;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    str_len = strlen(str);

    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        goto fail;
    }
    regex_compiled = 1;

    if (capacity > SIZE_MAX / sizeof(char *)) {
        goto fail;
    }
    result = (char **)malloc(capacity * sizeof(char *));
    if (result == NULL) {
        goto fail;
    }

    while (current <= str_len) {
        if (n >= capacity) {
            if (capacity > SIZE_MAX / 2) {
                goto fail;
            }
            capacity *= 2;
            if (capacity > SIZE_MAX / sizeof(char *)) {
                goto fail;
            }
            char **temp = (char **)realloc(result, capacity * sizeof(char *));
            if (temp == NULL) {
                goto fail;
            }
            result = temp;
        }

        ret = regexec(&regex, str + current, 1, &match, 0);
        if (ret == 0 && match.rm_so != -1) {
            size_t len = match.rm_so;
            if (len > 0) {
                if (len == SIZE_MAX || len > (size_t)INT_MAX) {
                    goto fail;
                }
                result[n] = (char *)malloc(len + 1);
                if (result[n] == NULL) {
                    goto fail;
                }
                snprintf(result[n], len + 1, "%.*s", (int)len, str + current);
                n++;
            }
            if (match.rm_eo == 0) {
                current++;
            } else {
                current += match.rm_eo;
            }
        } else {
            size_t remaining_len = str_len - current;
            if (remaining_len > 0) {
                if (remaining_len == SIZE_MAX || remaining_len > (size_t)INT_MAX) {
                    goto fail;
                }
                result[n] = (char *)malloc(remaining_len + 1);
                if (result[n] == NULL) {
                    goto fail;
                }
                snprintf(result[n], remaining_len + 1, "%.*s", (int)remaining_len, str + current);
                n++;
            }
            break;
        }
    }

    if (n == 0) {
        free(result);
        result = NULL;
    } else {
        if (n < capacity) {
            result[n] = NULL;
            if (n + 1 <= SIZE_MAX / sizeof(char *)) {
                char **final_result = (char **)realloc(result, (n + 1) * sizeof(char *));
                if (final_result != NULL) {
                    result = final_result;
                }
            }
        } else {
            if (n + 1 > SIZE_MAX / sizeof(char *)) {
                goto fail;
            }
            char **final_result = (char **)realloc(result, (n + 1) * sizeof(char *));
            if (final_result == NULL) {
                goto fail;
            }
            final_result[n] = NULL;
            result = final_result;
        }
    }

    if (regex_compiled) {
        regfree(&regex);
    }
    *count = n;
    return result;

fail:
    if (result != NULL) {
        for (size_t i = 0; i < n; i++) {
            free(result[i]);
        }
        free(result);
    }
    if (regex_compiled) {
        regfree(&regex);
    }
    *count = 0;
    return NULL;
}

void free_split_result(char **result) {
    if (result != NULL) {
        for (size_t i = 0; result[i] != NULL; i++) {
            free(result[i]);
        }
        free(result);
    }
}

int main(void) {
    const char *str = "apple;banana|cherry,date";
    const char *pattern = "[;|,]+";
    size_t count = 0;

    char **tokens = split_string_regex(str, pattern, &count);
    if (tokens != NULL) {
        for (size_t i = 0; i < count; i++) {
            printf("%s\n", tokens[i]);
        }
        free_split_result(tokens);
    }

    return 0;
}