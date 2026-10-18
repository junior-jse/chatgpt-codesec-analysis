#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char *remove_non_alphanumeric(const char *input) {
    if (input == NULL) {
        return NULL;
    }

    regex_t regex;
    int reti = regcomp(&regex, "[^a-zA-Z0-9]", REG_EXTENDED);
    if (reti) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(input);
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
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(result + j, cursor, copy_len);
                j += copy_len;
            }
            cursor += match.rm_eo;
            if (match.rm_eo == 0) {
                cursor++;
            }
        } else {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t remaining = strlen(cursor);
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + j, cursor, remaining);
            j += remaining;
            break;
        }
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