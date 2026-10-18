/* Repair history (previous round):
 * - Flawfinder strlen was removed from split_string() and now appears in main().
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int split_string(const char *input, size_t input_len, char ***output, size_t *count)
{
    regex_t regex;
    regmatch_t match;
    const char *cursor = input;
    /* Possible weaknesses found:
     *  Null pointer addition
     *  Either the condition 'input==NULL' is redundant or there is pointer arithmetic with NULL pointer. [nullPointerArithmeticRedundantCheck]
     */
    const char *end = input + input_len;
    size_t capacity = 8;
    size_t n = 0;
    char **parts = NULL;
    int ret = 0;

    /* Possible weaknesses found:
     *  Assuming that condition 'input==NULL' is not redundant
     */
    if (input == NULL || output == NULL || count == NULL)
        return -1;

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
            char **temp = (char **)realloc(parts, capacity * 2 * sizeof(char *));
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

        if (len > 0)
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(parts[n], start, len);
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
    const char *input = "SplitThisStringAtUppercaseLetters";
    char **parts = NULL;
    size_t count = 0;
    size_t i;

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    if (split_string(input, strlen(input), &parts, &count) != 0) {
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