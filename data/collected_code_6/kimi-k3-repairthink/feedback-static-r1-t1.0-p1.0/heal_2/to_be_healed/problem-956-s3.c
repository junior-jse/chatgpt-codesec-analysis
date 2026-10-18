#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdint.h>

int split_string(const char *input, char ***output, size_t *count)
{
    regex_t regex;
    regmatch_t match;
    const char *cursor;
    size_t capacity = 8;
    size_t n = 0;
    size_t input_len;
    size_t remaining;
    char **parts = NULL;
    int ret = 0;

    if (input == NULL || output == NULL || count == NULL)
        return -1;

    *output = NULL;
    *count = 0;

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    input_len = strlen(input);
    remaining = input_len;
    cursor = input;

    parts = (char **)malloc(capacity * sizeof(char *));
    if (parts == NULL)
        return -1;

    ret = regcomp(&regex, "[A-Z]", REG_EXTENDED);
    if (ret != 0) {
        free(parts);
        return -1;
    }

    while (remaining > 0) {
        const char *start = cursor;
        size_t len;
        size_t part_size;
        size_t i;

        if (regexec(&regex, cursor, 1, &match, 0) == 0)
            len = (size_t)match.rm_so;
        else
            len = remaining;

        if (len > remaining) {
            for (i = 0; i < n; ++i)
                free(parts[i]);
            free(parts);
            regfree(&regex);
            return -1;
        }

        if (n == capacity) {
            char **temp;

            if (capacity > SIZE_MAX / (2 * sizeof(char *))) {
                for (i = 0; i < n; ++i)
                    free(parts[i]);
                free(parts);
                regfree(&regex);
                return -1;
            }

            temp = (char **)realloc(parts, capacity * 2 * sizeof(char *));
            if (temp == NULL) {
                for (i = 0; i < n; ++i)
                    free(parts[i]);
                free(parts);
                regfree(&regex);
                return -1;
            }
            parts = temp;
            capacity *= 2;
        }

        part_size = len + 1;
        parts[n] = (char *)malloc(part_size);
        if (parts[n] == NULL) {
            for (i = 0; i < n; ++i)
                free(parts[i]);
            free(parts);
            regfree(&regex);
            return -1;
        }

        if (len > 0 && len <= part_size - 1)
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(parts[n], start, part_size - 1);
        parts[n][part_size - 1] = '\0';
        ++n;

        cursor += len;
        remaining -= len;
        if (remaining > 0 && *cursor != '\0') {
            ++cursor;
            --remaining;
        }
    }

    regfree(&regex);
    *output = parts;
    *count = n;
    return 0;
}

int main(void)
{
    const char *input = "SplitThisStringAtUppercaseLetters";
    char **parts = NULL;
    size_t count = 0;
    size_t i;

    if (split_string(input, &parts, &count) != 0) {
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