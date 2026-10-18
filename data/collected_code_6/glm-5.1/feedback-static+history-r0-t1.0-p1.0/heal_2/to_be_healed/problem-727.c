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

    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }

    size_t max_matches = len;
    regmatch_t *matches = malloc(max_matches * sizeof(regmatch_t));
    if (matches == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t result_size = len + 1;
    char *result = malloc(result_size);
    if (result == NULL) {
        free(matches);
        regfree(&regex);
        return NULL;
    }

    size_t result_idx = 0;
    size_t str_offset = 0;

    while (str_offset < len) {
        ret = regexec(&regex, str + str_offset, 1, matches, 0);
        
        if (ret == REG_NOMATCH) {
            size_t remaining = len - str_offset;
            snprintf(result + result_idx, result_size - result_idx, "%s", str + str_offset);
            result_idx += remaining;
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
            if (result_idx + copy_len < result_size) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + result_idx, str + str_offset, copy_len);
                result_idx += copy_len;
            } else {
                break;
            }
        }

        str_offset += matches[0].rm_eo;
    }

    if (result_idx < result_size) {
        result[result_idx] = '\0';
    } else {
        result[result_size - 1] = '\0';
    }

    char *final_result = realloc(result, result_idx + 1);
    if (final_result == NULL) {
        final_result = result;
    }

    free(matches);
    regfree(&regex);

    return final_result;
}

int main(void) {
    const char *input = "H3ll0 W0rld! @2024 #C_Programming*()";
    char *output = remove_non_alnum(input);
    
    if (output != NULL) {
        printf("%s\n", output);
        free(output);
    }

    return 0;
}