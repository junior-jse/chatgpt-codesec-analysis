#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <limits.h>

typedef struct {
    char **items;
    size_t count;
} StringArray;

static void free_string_array(StringArray *array)
{
    if (array == NULL) {
        return;
    }

    for (size_t i = 0; i < array->count; ++i) {
        free(array->items[i]);
    }

    free(array->items);
    array->items = NULL;
    array->count = 0;
}

static int append_token(StringArray *array, const char *start, size_t length)
{
    if (array == NULL || (start == NULL && length != 0)) {
        return -1;
    }

    if (array->count == SIZE_MAX / sizeof(*array->items)) {
        return -1;
    }

    char *token = malloc(length + 1);
    if (token == NULL) {
        return -1;
    }

    if (length != 0) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(token, start, length);
    }
    token[length] = '\0';

    char **new_items = realloc(
        array->items,
        (array->count + 1) * sizeof(*array->items)
    );

    if (new_items == NULL) {
        free(token);
        return -1;
    }

    array->items = new_items;
    array->items[array->count++] = token;
    return 0;
}

static int split_regex(const char *input, const char *delimiter_pattern,
                       StringArray *result)
{
    if (input == NULL || delimiter_pattern == NULL || result == NULL) {
        return -1;
    }

    result->items = NULL;
    result->count = 0;

    regex_t regex;
    int status = regcomp(&regex, delimiter_pattern, REG_EXTENDED);
    if (status != 0) {
        return -1;
    }

    const char *cursor = input;

    while (*cursor != '\0') {
        regmatch_t match;
        status = regexec(&regex, cursor, 1, &match, 0);

        if (status == REG_NOMATCH) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            if (append_token(result, cursor, strlen(cursor)) != 0) {
                regfree(&regex);
                free_string_array(result);
                return -1;
            }
            /* Possible weaknesses found:
             *  Variable 'cursor' is assigned a value that is never used. [unreadVariable]
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             *  Value stored to 'cursor' is never read [deadcode.DeadStores]
             */
            cursor += strlen(cursor);
            break;
        }

        if (status != 0 || match.rm_so < 0 || match.rm_eo < match.rm_so) {
            regfree(&regex);
            free_string_array(result);
            return -1;
        }

        size_t token_length = (size_t)match.rm_so;
        size_t delimiter_length = (size_t)(match.rm_eo - match.rm_so);

        if (token_length != 0 &&
            append_token(result, cursor, token_length) != 0) {
            regfree(&regex);
            free_string_array(result);
            return -1;
        }

        if (delimiter_length == 0) {
            regfree(&regex);
            free_string_array(result);
            return -1;
        }

        cursor += (size_t)match.rm_eo;
    }

    regfree(&regex);
    return 0;
}

int main(void)
{
    const char *input = "one,two;three four\tfive";
    const char *delimiters = "[,;[:space:]]+";
    StringArray tokens = {0};

    if (split_regex(input, delimiters, &tokens) != 0) {
        fputs("Failed to split string\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < tokens.count; ++i) {
        puts(tokens.items[i]);
    }

    free_string_array(&tokens);
    return EXIT_SUCCESS;
}