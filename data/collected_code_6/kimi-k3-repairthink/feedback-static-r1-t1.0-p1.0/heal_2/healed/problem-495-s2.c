#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_STRING_LENGTH 1024

int remove_lowercase_substrings(char *str, size_t str_size) {
    regex_t regex;
    regmatch_t match;
    size_t read_pos = 0;
    size_t write_pos = 0;
    size_t str_len;
    size_t remaining;
    size_t prefix_len;
    int ret;

    if (str == NULL || str_size == 0) {
        return -1;
    }

    str_len = strnlen(str, str_size);
    if (str_len >= str_size) {
        return -1;
    }

    ret = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (ret != 0) {
        return -1;
    }

    while (read_pos < str_len) {
        ret = regexec(&regex, str + read_pos, 1, &match, 0);

        if (ret == REG_NOMATCH) {
            remaining = str_len - read_pos;
            if (write_pos < read_pos) {
                memmove(str + write_pos, str + read_pos, remaining);
            }
            write_pos += remaining;
            break;
        } else if (ret != 0) {
            regfree(&regex);
            return -1;
        }

        if (match.rm_so > 0) {
            prefix_len = (size_t)match.rm_so;
            if (write_pos < read_pos) {
                memmove(str + write_pos, str + read_pos, prefix_len);
            }
            write_pos += prefix_len;
        }

        if (match.rm_eo <= 0) {
            break;
        }

        read_pos += (size_t)match.rm_eo;
    }

    str[write_pos] = '\0';

    regfree(&regex);
    return 0;
}

int main(void) {
    char *test_string;
    int written;

    test_string = malloc(MAX_STRING_LENGTH);
    if (test_string == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    written = snprintf(test_string, MAX_STRING_LENGTH, "%s", "HelloWorldABCDefgHIJKlmnOPQ");
    if (written < 0 || (size_t)written >= MAX_STRING_LENGTH) {
        fprintf(stderr, "Failed to initialize string\n");
        free(test_string);
        return EXIT_FAILURE;
    }

    printf("Original string: %s\n", test_string);

    if (remove_lowercase_substrings(test_string, MAX_STRING_LENGTH) == 0) {
        printf("Modified string: %s\n", test_string);
    } else {
        fprintf(stderr, "Error processing string\n");
        free(test_string);
        return EXIT_FAILURE;
    }

    free(test_string);
    return EXIT_SUCCESS;
}