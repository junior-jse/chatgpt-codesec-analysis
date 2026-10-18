#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdint.h>

char *put_spaces(const char *str) {
    if (!str) return NULL;

    regex_t regex;
    regmatch_t match;
    int err;
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str);
    if (len == SIZE_MAX) return NULL;
    
    size_t capacity;
    if (len > SIZE_MAX / 2 - 1) {
        capacity = len + 1;
    } else {
        capacity = len * 2 + 1;
    }
    
    char *result = malloc(capacity);
    if (!result) return NULL;

    result[0] = '\0';
    size_t result_len = 0;

    err = regcomp(&regex, "[A-Z][a-z]*", REG_EXTENDED);
    if (err != 0) {
        free(result);
        return NULL;
    }

    const char *p = str;

    while (1) {
        err = regexec(&regex, p, 1, &match, 0);
        if (err == REG_NOMATCH) break;
        if (err != 0) {
            regfree(&regex);
            free(result);
            return NULL;
        }

        if (match.rm_so == -1) break;

        size_t prefix_len = match.rm_so;
        size_t match_len = match.rm_eo - match.rm_so;
        size_t space_len = (result_len > 0) ? 1 : 0;
        size_t needed = result_len + prefix_len + space_len + match_len + 1;

        if (needed > capacity) {
            capacity = (needed <= SIZE_MAX / 2) ? needed * 2 : needed;
            char *temp = realloc(result, capacity);
            if (!temp) {
                regfree(&regex);
                free(result);
                return NULL;
            }
            result = temp;
        }

        if (prefix_len > 0) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + result_len, p, prefix_len);
            result_len += prefix_len;
        }

        if (space_len > 0) {
            result[result_len++] = ' ';
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + result_len, p + match.rm_so, match_len);
        result_len += match_len;

        p += match.rm_eo;
    }

    size_t remaining_len = len - (p - str);
    if (remaining_len > 0) {
        size_t needed = result_len + remaining_len + 1;
        if (needed > capacity) {
            capacity = needed;
            char *temp = realloc(result, capacity);
            if (!temp) {
                regfree(&regex);
                free(result);
                return NULL;
            }
            result = temp;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + result_len, p, remaining_len);
        result_len += remaining_len;
    }

    result[result_len] = '\0';

    regfree(&regex);

    char *final_result = realloc(result, result_len + 1);
    if (!final_result) return result;

    return final_result;
}

int main(int argc, const char * const argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    char *output = put_spaces(argv[1]);
    if (!output) {
        fprintf(stderr, "Memory or regex error\n");
        return 1;
    }

    printf("%s\n", output);

    free(output);
    return 0;
}