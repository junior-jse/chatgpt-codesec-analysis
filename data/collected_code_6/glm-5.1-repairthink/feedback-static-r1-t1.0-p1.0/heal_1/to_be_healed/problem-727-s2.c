#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *remove_non_alnum(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    regex_t regex;
    int ret = regcomp(&regex, "[^a-zA-Z0-9]", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t max_matches = strlen(str);
    regmatch_t *matches = malloc(max_matches * sizeof(regmatch_t));
    if (matches == NULL) {
        regfree(&regex);
        return NULL;
    }

    char *result = malloc(max_matches + 1);
    if (result == NULL) {
        free(matches);
        regfree(&regex);
        return NULL;
    }

    /* Possible weaknesses found:
     *  Variable 'src_idx' is assigned a value that is never used. [unreadVariable]
     */
    size_t src_idx = 0;
    size_t dest_idx = 0;
    const char *current_ptr = str;

    while (1) {
        ret = regexec(&regex, current_ptr, 1, matches, 0);
        if (ret == REG_NOMATCH) {
            break;
        }
        if (ret != 0) {
            free(result);
            free(matches);
            regfree(&regex);
            return NULL;
        }

        if (matches[0].rm_so == -1) {
            break;
        }

        size_t copy_len = matches[0].rm_so;
        if (copy_len > 0) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + dest_idx, current_ptr, copy_len);
            dest_idx += copy_len;
        }

        current_ptr += matches[0].rm_eo;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t remaining_len = strlen(current_ptr);
    if (remaining_len > 0) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + dest_idx, current_ptr, remaining_len);
        dest_idx += remaining_len;
    }

    result[dest_idx] = '\0';

    free(matches);
    regfree(&regex);

    char *shrunk_result = realloc(result, dest_idx + 1);
    if (shrunk_result == NULL) {
        if (dest_idx > 0 || result[0] != '\0') {
            return result;
        }
        free(result);
        return NULL;
    }

    return shrunk_result;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    char *filtered = remove_non_alnum(argv[1]);
    if (filtered == NULL) {
        fprintf(stderr, "Memory allocation or regex error\n");
        return 1;
    }

    printf("%s\n", filtered);
    free(filtered);

    return 0;
}