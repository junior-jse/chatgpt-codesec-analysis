#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdint.h>

int split_string(const char *input, size_t input_len, char ***output, size_t *count)
{
    regex_t regex;
    regmatch_t match;
    const char *cursor;
    const char *end;
    size_t capacity = 8;
    size_t n = 0;
    char **parts = NULL;
    int ret = 0;

    if (input == NULL || output == NULL || count == NULL)
        return -1;

    cursor = input;
    end = input + input_len;

    *output = NULL;
    *count = 0;

    parts = (char **)malloc(capacity * sizeof(char *));
    if (parts == NULL)
        return -1;

    ret = regcomp(&regex, "[A-Z]", REG_EXTENDED);
    if (ret != 0) {
        free(parts);
        return -1;
    }

    while (cursor < end && *cursor != '\0') {
        const char *start = cursor;
        size_t len;
        size_t remaining = (size_t)(end - cursor);

        if (regexec(&regex, cursor, 1, &match, 0) == 0 && (size_t)match.rm_so <= remaining)
            len = (size_t)match.rm_so;
        else
            len = strnlen(cursor, remaining);

        if (n == capacity) {
            char **temp;
            if (capacity > SIZE_MAX / 2 / sizeof(char *)) {
                size_t i;
                for (i = 0; i < n; ++i)
                    free(parts[i]);
                free(parts);
                regfree(&regex);
                return -1;
            }
            temp = (char **)realloc(parts, capacity * 2 * sizeof(char *));
            if (temp == NULL) {
                size_t i;
                for (i = 0; i < n; ++i)
                    free(parts[i]);
                free(parts);
                regfree(&regex);
                return -1;
            }
            parts = temp;
            capacity *= 2;
        }

        if (len > SIZE_MAX - 1) {
            size_t i;
            for (i = 0; i < n; ++i)
                free(parts[i]);
            free(parts);
            regfree(&regex);
            return -1;
        }

        parts[n] = (char *)malloc(len + 1);
        if (parts[n] == NULL) {
            size_t i;
            for (i = 0; i < n; ++i)
                free(parts[i]);
            free(parts);
            regfree(&regex);
            return -1;
        }

        if (len > 0) {
            if (len > SIZE_MAX - 1) {
                size_t i;
                for (i = 0; i < n; ++i)
                    free(parts[i]);
                free(parts[n]);
                free(parts);
                regfree(&regex);
                return -1;
            }
            if (len > remaining) {
                size_t i;
                for (i = 0; i < n; ++i)
                    free(parts[i]);
                free(parts[n]);
                free(parts);
                regfree(&regex);
                return -1;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(parts[n], start, len);
        }
        parts[n][len] = '\0';
        ++n;

        cursor += len;
        if (cursor < end && *cursor != '\0')
            ++cursor;
    }

    regfree(&regex);
    *output = parts;
    *count = n;
    return 0;
}

int main(void)
{
    const char input[] = "SplitThisStringAtUppercaseLetters";
    char **parts = NULL;
    size_t count = 0;
    size_t i;

    if (split_string(input, sizeof(input) - 1, &parts, &count) != 0) {
        fprintf(stderr, "Failed to split string\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        printf("%s\n", parts[i]);
        free(parts[i]);
    }

    free(parts);
    return EXIT_SUCCESS;
}