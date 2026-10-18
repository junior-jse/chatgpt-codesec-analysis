#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_STRING_LENGTH 1024

int replace_chars_with_colon(char *str, size_t buffer_size) {
    regex_t regex;
    regmatch_t match;
    char *result;
    size_t str_len;
    size_t result_len = 0;
    size_t pos = 0;
    int ret;

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
            free(result);
            regfree(&regex);
            return -1;
        }

        if (match.rm_so < 0 || match.rm_eo < 0 || match.rm_eo < match.rm_so) {
            free(result);
            regfree(&regex);
            return -1;
        }

        size_t copy_len = (size_t)match.rm_so;
        if (copy_len > buffer_size || result_len > buffer_size - copy_len - 2) {
            free(result);
            regfree(&regex);
            return -1;
        }

        if (copy_len > 0) {
            memcpy(result + result_len, str + pos, copy_len);
            result_len += copy_len;
        }
        result[result_len++] = ':';
        result[result_len] = '\0';

        pos += (size_t)match.rm_eo;
    }

    size_t remaining = str_len - pos;
    if (remaining > buffer_size || result_len > buffer_size - remaining - 1) {
        free(result);
        regfree(&regex);
        return -1;
    }

    if (remaining > 0) {
        memcpy(result + result_len, str + pos, remaining);
        result_len += remaining;
    }
    result[result_len] = '\0';

    if (result_len < buffer_size) {
        memcpy(str, result, result_len + 1);
    } else {
        free(result);
        regfree(&regex);
        return -1;
    }

    free(result);
    regfree(&regex);

    return 0;
}

int main(void) {
    char test_string[MAX_STRING_LENGTH] = "Hello, World. This is a test string, with spaces.";
    int status;

    printf("Original string: %s\n", test_string);

    status = replace_chars_with_colon(test_string, sizeof(test_string));
    if (status != 0) {
        fprintf(stderr, "Error: Failed to replace characters\n");
        return EXIT_FAILURE;
    }

    printf("Modified string: %s\n", test_string);

    return EXIT_SUCCESS;
}