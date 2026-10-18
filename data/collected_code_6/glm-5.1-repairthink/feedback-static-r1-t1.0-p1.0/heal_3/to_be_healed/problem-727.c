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
    regmatch_t matches;

    size_t result_size = max_matches + 1;
    char *result = malloc(result_size);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t result_idx = 0;
    size_t str_offset = 0;

    while (str_offset < max_matches) {
        ret = regexec(&regex, str + str_offset, 1, &matches, 0);
        
        if (ret == REG_NOMATCH) {
            size_t rem_len = max_matches - str_offset;
            size_t available = result_size - result_idx;
            if (available > 0) {
                size_t safe_copy_len = rem_len < available ? rem_len : available;
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + result_idx, str + str_offset, safe_copy_len);
            }
            result_idx += rem_len;
            break;
        }

        if (ret != 0) {
            free(result);
            regfree(&regex);
            return NULL;
        }

        if (matches.rm_so == -1) {
            break;
        }

        size_t copy_len = matches.rm_so;
        if (copy_len > 0) {
            size_t available = result_size - result_idx;
            if (available > 0) {
                size_t safe_copy_len = copy_len < available ? copy_len : available;
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + result_idx, str + str_offset, safe_copy_len);
            }
            result_idx += copy_len;
        }

        str_offset += matches.rm_eo;
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