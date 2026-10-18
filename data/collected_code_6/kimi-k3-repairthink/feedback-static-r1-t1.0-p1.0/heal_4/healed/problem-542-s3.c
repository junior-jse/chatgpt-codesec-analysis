#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <regex.h>

#define MAX_SOURCE_LENGTH 4096

int replace_chars_with_colon(char *str, size_t buffer_size) {
    regex_t regex;
    regmatch_t match;
    size_t str_len;
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

    while (pos < str_len) {
        ret = regexec(&regex, str + pos, 1, &match, 0);
        if (ret == REG_NOMATCH) {
            break;
        }
        if (ret != 0) {
            regfree(&regex);
            return -1;
        }

        if (match.rm_so < 0 || match.rm_eo <= match.rm_so ||
            (size_t)match.rm_eo > str_len - pos) {
            regfree(&regex);
            return -1;
        }

        str[pos + (size_t)match.rm_so] = ':';
        pos += (size_t)match.rm_eo;
    }

    regfree(&regex);
    return 0;
}

int main(int argc, char *argv[]) {
    const char *source;
    size_t source_len;
    size_t buffer_size;
    char *test_string;
    int status;
    int written;

    source = (argc > 1) ? argv[1] : "Hello, World. This is a test string, with spaces.";

    source_len = strnlen(source, MAX_SOURCE_LENGTH);
    if (source_len == 0 || source_len >= MAX_SOURCE_LENGTH) {
        fprintf(stderr, "Error: Invalid source string length\n");
        return EXIT_FAILURE;
    }

    buffer_size = source_len + 1;

    test_string = malloc(buffer_size);
    if (test_string == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    written = snprintf(test_string, buffer_size, "%s", source);
    if (written < 0 || (size_t)written >= buffer_size) {
        fprintf(stderr, "Error: Failed to copy source string\n");
        free(test_string);
        return EXIT_FAILURE;
    }

    printf("Original string: %s\n", test_string);

    status = replace_chars_with_colon(test_string, buffer_size);
    if (status != 0) {
        fprintf(stderr, "Error: Failed to replace characters\n");
        free(test_string);
        return EXIT_FAILURE;
    }

    printf("Modified string: %s\n", test_string);

    free(test_string);
    return EXIT_SUCCESS;
}