#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_MATCHES 100

char **split_at_uppercase(const char *str, size_t *count) {
    regex_t regex;
    regmatch_t matches[MAX_MATCHES];
    int ret;
    char **result = NULL;
    *count = 0;

    if (str == NULL) {
        return NULL;
    }

    ret = regcomp(&regex, "[A-Z]", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    size_t capacity = 10;
    result = (char **)malloc(capacity * sizeof(char *));
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    const char *p = str;
    /* Possible weaknesses found:
     *  Variable 'start' is assigned a value that is never used. [unreadVariable]
     */
    size_t start = 0;
    size_t i = 0;

    while (regexec(&regex, p, MAX_MATCHES, matches, 0) == 0) {
        size_t prefix_len = matches[0].rm_so;
        size_t match_end_in_p = matches[0].rm_eo;

        if (i >= capacity) {
            capacity *= 2;
            char **temp = (char **)realloc(result, capacity * sizeof(char *));
            if (temp == NULL) {
                for (size_t j = 0; j < i; j++) {
                    free(result[j]);
                }
                free(result);
                regfree(&regex);
                return NULL;
            }
            result = temp;
        }

        if (prefix_len > 0) {
            result[i] = (char *)malloc(prefix_len + 1);
            if (result[i] == NULL) {
                for (size_t j = 0; j < i; j++) {
                    free(result[j]);
                }
                free(result);
                regfree(&regex);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result[i], p, prefix_len);
            result[i][prefix_len] = '\0';
            i++;
        }

        if (i >= capacity) {
            capacity *= 2;
            char **temp = (char **)realloc(result, capacity * sizeof(char *));
            if (temp == NULL) {
                for (size_t j = 0; j < i; j++) {
                    free(result[j]);
                }
                free(result);
                regfree(&regex);
                return NULL;
            }
            result = temp;
        }

        size_t upper_len = match_end_in_p - matches[0].rm_so;
        result[i] = (char *)malloc(upper_len + 1);
        if (result[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            regfree(&regex);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[i], p + matches[0].rm_so, upper_len);
        result[i][upper_len] = '\0';
        i++;

        p += match_end_in_p;
        /* Possible weaknesses found:
         *  Variable 'start' is assigned a value that is never used. [unreadVariable]
         */
        start += match_end_in_p;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t remaining_len = strlen(p);
    if (remaining_len > 0) {
        if (i >= capacity) {
            capacity *= 2;
            char **temp = (char **)realloc(result, capacity * sizeof(char *));
            if (temp == NULL) {
                for (size_t j = 0; j < i; j++) {
                    free(result[j]);
                }
                free(result);
                regfree(&regex);
                return NULL;
            }
            result = temp;
        }

        result[i] = (char *)malloc(remaining_len + 1);
        if (result[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            regfree(&regex);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[i], p, remaining_len);
        result[i][remaining_len] = '\0';
        i++;
    }

    regfree(&regex);
    *count = i;
    return result;
}

int main(void) {
    const char *input = "SplitThisStringAtUppercaseLetters";
    size_t count = 0;
    char **parts = split_at_uppercase(input, &count);

    if (parts != NULL) {
        for (size_t i = 0; i < count; i++) {
            printf("%s\n", parts[i]);
            free(parts[i]);
        }
        free(parts);
    }

    return 0;
}