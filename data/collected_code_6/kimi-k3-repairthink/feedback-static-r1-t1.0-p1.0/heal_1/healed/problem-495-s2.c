#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_STRING_LENGTH 1024

int remove_lowercase_substrings(char *str, size_t str_size) {
    regex_t regex;
    regmatch_t match;
    char *result;
    size_t result_len = 0;
    size_t offset = 0;
    size_t str_len;
    int ret;

    if (str == NULL || str_size == 0) {
        return -1;
    }

    str_len = strnlen(str, str_size);
    if (str_len >= str_size) {
        return -1;
    }

    result = malloc(str_size);
    if (result == NULL) {
        return -1;
    }

    ret = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (ret != 0) {
        free(result);
        return -1;
    }

    while (offset < str_len) {
        ret = regexec(&regex, str + offset, 1, &match, 0);

        if (ret == REG_NOMATCH) {
            size_t remaining = str_len - offset;
            if (remaining >= str_size - result_len) {
                regfree(&regex);
                free(result);
                return -1;
            }
            memcpy(result + result_len, str + offset, remaining);
            result_len += remaining;
            break;
        } else if (ret != 0) {
            regfree(&regex);
            free(result);
            return -1;
        }

        if (match.rm_so > 0) {
            size_t prefix_len = (size_t)match.rm_so;
            if (prefix_len >= str_size - result_len) {
                regfree(&regex);
                free(result);
                return -1;
            }
            memcpy(result + result_len, str + offset, prefix_len);
            result_len += prefix_len;
        }

        offset += (size_t)match.rm_eo;
    }

    if (result_len >= str_size) {
        regfree(&regex);
        free(result);
        return -1;
    }
    result[result_len] = '\0';

    memcpy(str, result, result_len + 1);

    regfree(&regex);
    free(result);
    return 0;
}

int main(void) {
    char test_string[MAX_STRING_LENGTH] = "HelloWorldABCDefgHIJKlmnOPQ";

    printf("Original string: %s\n", test_string);

    if (remove_lowercase_substrings(test_string, sizeof(test_string)) == 0) {
        printf("Modified string: %s\n", test_string);
    } else {
        fprintf(stderr, "Error processing string\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}