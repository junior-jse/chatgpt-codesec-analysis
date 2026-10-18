#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdint.h>

#define MAX_INPUT_LEN ((size_t)1048576)

static void free_parts(char **parts, size_t n)
{
    size_t i;

    if (parts == NULL)
        return;

    for (i = 0; i < n; ++i)
        free(parts[i]);
    free(parts);
}

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
    char *input_copy = NULL;
    int ret = 0;

    if (input == NULL || output == NULL || count == NULL)
        return -1;

    *output = NULL;
    *count = 0;

    input_len = strnlen(input, MAX_INPUT_LEN);

    if (input_len >= SIZE_MAX)
        return -1;

    input_copy = (char *)malloc(input_len + 1);
    if (input_copy == NULL)
        return -1;

    if (input_len > 0) {
        if (input_len > (input_len + 1) - 1) {
            free(input_copy);
            return -1;
        }
        memcpy(input_copy, input, input_len);
    }
    input_copy[input_len] = '\0';

    remaining = input_len;
    cursor = input_copy;

    if (capacity > SIZE_MAX / sizeof(char *)) {
        free(input_copy);
        return -1;
    }

    parts = (char **)malloc(capacity * sizeof(char *));
    if (parts == NULL) {
        free(input_copy);
        return -1;
    }

    ret = regcomp(&regex, "[A-Z]", REG_EXTENDED);
    if (ret != 0) {
        free(parts);
        free(input_copy);
        return -1;
    }

    while (remaining > 0) {
        const char *start = cursor;
        size_t len;
        size_t part_size;

        if (regexec(&regex, cursor, 1, &match, 0) == 0 && match.rm_so >= 0)
            len = (size_t)match.rm_so;
        else
            len = remaining;

        if (len > remaining || (size_t)(cursor - input_copy) + len > input_len) {
            free_parts(parts, n);
            regfree(&regex);
            free(input_copy);
            return -1;
        }

        if (n == capacity) {
            char **temp;

            if (capacity > SIZE_MAX / (2 * sizeof(char *))) {
                free_parts(parts, n);
                regfree(&regex);
                free(input_copy);
                return -1;
            }

            temp = (char **)realloc(parts, capacity * 2 * sizeof(char *));
            if (temp == NULL) {
                free_parts(parts, n);
                regfree(&regex);
                free(input_copy);
                return -1;
            }
            parts = temp;
            capacity *= 2;
        }

        if (len >= SIZE_MAX) {
            free_parts(parts, n);
            regfree(&regex);
            free(input_copy);
            return -1;
        }

        part_size = len + 1;

        parts[n] = (char *)malloc(part_size);
        if (parts[n] == NULL) {
            free_parts(parts, n);
            regfree(&regex);
            free(input_copy);
            return -1;
        }

        if (len > 0) {
            if (len > part_size - 1) {
                free_parts(parts, n + 1);
                regfree(&regex);
                free(input_copy);
                return -1;
            }
            memcpy(parts[n], start, len);
        }
        parts[n][len] = '\0';
        ++n;

        cursor += len;
        remaining -= len;
        if (remaining > 0 && *cursor != '\0') {
            ++cursor;
            --remaining;
        }
    }

    regfree(&regex);
    free(input_copy);
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