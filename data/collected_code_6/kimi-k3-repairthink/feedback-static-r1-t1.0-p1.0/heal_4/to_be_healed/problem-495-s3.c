#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_INPUT_LENGTH 1048576

static int safe_append(char *dest, size_t dest_capacity, size_t *dest_len,
                       const char *src, size_t src_len)
{
    if (dest == NULL || dest_len == NULL || src == NULL) {
        return -1;
    }
    if (*dest_len >= dest_capacity) {
        return -1;
    }
    if (src_len > dest_capacity - *dest_len - 1) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dest + *dest_len, src, src_len);
    *dest_len += src_len;
    dest[*dest_len] = '\0';
    return 0;
}

char *remove_lowercase_substrings(const char *input)
{
    regex_t regex;
    regmatch_t match;
    const char *cursor;
    char *result;
    size_t result_len;
    size_t result_capacity;
    size_t input_len;
    /* Possible weaknesses found:
     *  The scope of the variable 'remaining' can be reduced. [variableScope]
     */
    size_t remaining;
    int ret;

    if (input == NULL) {
        return NULL;
    }

    input_len = strnlen(input, MAX_INPUT_LENGTH);
    if (input_len == MAX_INPUT_LENGTH) {
        return NULL;
    }

    ret = regcomp(&regex, "[a-z]+", REG_EXTENDED);
    if (ret != 0) {
        return NULL;
    }

    result_capacity = input_len + 1;
    result = (char *)malloc(result_capacity);
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    cursor = input;
    result_len = 0;

    while (*cursor != '\0') {
        remaining = input_len - (size_t)(cursor - input);

        ret = regexec(&regex, cursor, 1, &match, 0);
        if (ret == REG_NOMATCH) {
            if (safe_append(result, result_capacity, &result_len,
                            cursor, strnlen(cursor, remaining)) != 0) {
                free(result);
                regfree(&regex);
                return NULL;
            }
            break;
        } else if (ret != 0) {
            free(result);
            regfree(&regex);
            return NULL;
        }

        if (match.rm_so < 0 || match.rm_eo < 0 ||
            match.rm_eo < match.rm_so ||
            (size_t)match.rm_eo > remaining) {
            free(result);
            regfree(&regex);
            return NULL;
        }

        if (safe_append(result, result_capacity, &result_len,
                        cursor, (size_t)match.rm_so) != 0) {
            free(result);
            regfree(&regex);
            return NULL;
        }

        if (match.rm_eo == 0) {
            free(result);
            regfree(&regex);
            return NULL;
        }

        cursor += match.rm_eo;
    }

    result[result_len] = '\0';
    regfree(&regex);
    return result;
}

int main(void)
{
    const char *test_string = "HelloWorldTESTString123abcXYZ";
    char *processed;

    processed = remove_lowercase_substrings(test_string);
    if (processed == NULL) {
        fprintf(stderr, "Error processing string\n");
        return EXIT_FAILURE;
    }

    printf("Original: %s\n", test_string);
    printf("Processed: %s\n", processed);

    free(processed);
    return EXIT_SUCCESS;
}