#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>

#define MAX_STRING_LENGTH 1024

int replace_chars_with_colon(char *str, size_t buffer_size) {
    regex_t regex;
    regmatch_t match;
    char *result;
    size_t str_len;
    size_t result_len = 0;
    size_t pos = 0;
    int ret;
    int status = -1;

    if (str == NULL || buffer_size == 0) {
        return -1;
    }

    str_len = strnlen(str, buffer_size);
    if (str_len >= buffer_size) {
        return -1;
    }

    ret = regcomp(&regex, "[ ,.]", REG_EXTENDED);
    if (ret != 0) {
        return -1;
    }

    result = (char *)malloc(buffer_size);
    if (result == NULL) {
        regfree(&regex);
        return -1;
    }

    result[0] = '\0';

    while (pos < str_len) {
        ret = regexec(&regex, str + pos, 1, &match, 0);
        if (ret == REG_NOMATCH) {
            break;
        } else if (ret != 0) {
            goto cleanup;
        }

        if (match.rm_so < 0 || match.rm_eo < 0 || match.rm_eo < match.rm_so) {
            goto cleanup;
        }

        size_t copy_len = (size_t)match.rm_so;
        
        if (copy_len > buffer_size || result_len > buffer_size - copy_len - 2) {
            goto cleanup;
        }

        if (copy_len > 0) {
            if (result_len + copy_len >= buffer_size) {
                goto cleanup;
            }
            if (result_len + copy_len > buffer_size - 1) {
                goto cleanup;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + result_len, str + pos, copy_len);
            result_len += copy_len;
        }
        
        if (result_len + 1 >= buffer_size) {
            goto cleanup;
        }
        result[result_len++] = ':';
        result[result_len] = '\0';

        pos += (size_t)match.rm_eo;
    }

    size_t remaining = str_len - pos;
    
    if (remaining > buffer_size || result_len > buffer_size - remaining - 1) {
        goto cleanup;
    }

    if (remaining > 0) {
        if (result_len + remaining >= buffer_size) {
            goto cleanup;
        }
        if (result_len + remaining > buffer_size - 1) {
            goto cleanup;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + result_len, str + pos, remaining);
        result_len += remaining;
    }
    result[result_len] = '\0';

    if (result_len < buffer_size) {
        /* Possible weaknesses found:
         *  First condition
         */
        if (result_len + 1 > buffer_size) {
            goto cleanup;
        }
        /* Possible weaknesses found:
         *  Second condition
         *  The if condition is the same as the previous if condition [duplicateCondition]
         */
        if (result_len + 1 > buffer_size) {
            goto cleanup;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(str, result, result_len + 1);
        status = 0;
    }

cleanup:
    free(result);
    regfree(&regex);
    return status;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char test_string[MAX_STRING_LENGTH];
    int status;
    
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). Risk is low because the source is a constant string. (risk 1, buffer)
     */
    strncpy(test_string, "Hello, World. This is a test string, with spaces.", MAX_STRING_LENGTH - 1);
    test_string[MAX_STRING_LENGTH - 1] = '\0';

    printf("Original string: %s\n", test_string);

    status = replace_chars_with_colon(test_string, sizeof(test_string));
    if (status != 0) {
        fprintf(stderr, "Error: Failed to replace characters\n");
        return EXIT_FAILURE;
    }

    printf("Modified string: %s\n", test_string);

    return EXIT_SUCCESS;
}