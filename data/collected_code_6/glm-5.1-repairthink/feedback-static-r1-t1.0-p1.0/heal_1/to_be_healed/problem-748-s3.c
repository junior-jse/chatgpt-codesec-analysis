#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *put_spaces(const char *str) {
    regex_t regex;
    regmatch_t match;
    int err;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t capacity = strlen(str) * 2 + 1;
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

        if (result_len + prefix_len + match_len + 2 >= capacity) {
            capacity = (result_len + prefix_len + match_len + 2) * 2;
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

        if (result_len > 0) {
            result[result_len++] = ' ';
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + result_len, p + match.rm_so, match_len);
        result_len += match_len;

        p += match.rm_eo;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t remaining_len = strlen(p);
    if (remaining_len > 0) {
        if (result_len + remaining_len + 1 >= capacity) {
            capacity = result_len + remaining_len + 1;
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

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[]) {
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