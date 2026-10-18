#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdint.h>

char *remove_non_alphanumeric(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    regex_t regex;
    int reti = regcomp(&regex, "[^a-zA-Z0-9]", REG_EXTENDED);
    if (reti) {
        return NULL;
    }

    size_t len = strnlen(input, SIZE_MAX);
    if (len == SIZE_MAX) {
        regfree(&regex);
        return NULL;
    }

    char *result = (char *)malloc(len + 1);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    size_t j = 0;
    regmatch_t match;
    const char *cursor = input;

    while (*cursor != '\0') {
        if (regexec(&regex, cursor, 1, &match, 0) == 0) {
            if (match.rm_so > 0) {
                size_t copy_len = (size_t)match.rm_so;
                if (j + copy_len > len) {
                    copy_len = len - j;
                }
                if (copy_len > 0) {
                    memcpy(result + j, cursor, copy_len);
                    j += copy_len;
                }
            }
            cursor += match.rm_eo;
            if (match.rm_eo == 0) {
                cursor++;
            }
        } else {
            size_t remaining = strnlen(cursor, len - j + 1);
            if (j + remaining > len) {
                remaining = len - j;
            }
            if (remaining > 0) {
                memcpy(result + j, cursor, remaining);
                j += remaining;
            }
            break;
        }
    }

    if (j > len) {
        j = len;
    }
    result[j] = '\0';
    regfree(&regex);
    return result;
}

int main(void) {
    const char *test_cases[] = {
        "Hello, World! 123",
        "C@de#2024$",
        "   Spaces   ",
        "NoSpecialChars123",
        "",
        "!@#$%^&*()",
        NULL
    };

    for (int i = 0; test_cases[i] != NULL; i++) {
        char *cleaned = remove_non_alphanumeric(test_cases[i]);
        if (cleaned != NULL) {
            printf("Input: \"%s\" -> Output: \"%s\"\n", test_cases[i], cleaned);
            free(cleaned);
        } else {
            printf("Failed to process: \"%s\"\n", test_cases[i]);
        }
    }

    return 0;
}