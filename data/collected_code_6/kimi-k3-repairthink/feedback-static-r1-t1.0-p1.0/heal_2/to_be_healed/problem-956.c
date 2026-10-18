#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_INPUT_LENGTH ((size_t)65536)

static size_t bounded_length(const char *s, size_t max)
{
    size_t i = 0;

    while (i < max && s[i] != '\0') {
        i++;
    }
    return i;
}

int split_at_uppercase(const char *input, char ***tokens, size_t *count)
{
    regex_t regex;
    regmatch_t match;
    const char *cursor;
    size_t capacity = 8;
    size_t n = 0;
    char **result = NULL;
    char **temp = NULL;
    size_t start;
    size_t len;
    size_t input_len;
    size_t remaining;
    size_t buf_size;

    if (input == NULL || tokens == NULL || count == NULL) {
        return -1;
    }

    input_len = bounded_length(input, MAX_INPUT_LENGTH + 1);
    if (input_len > MAX_INPUT_LENGTH) {
        return -1;
    }

    if (regcomp(&regex, "[A-Z]", REG_EXTENDED) != 0) {
        return -1;
    }

    result = malloc(capacity * sizeof(*result));
    if (result == NULL) {
        regfree(&regex);
        return -1;
    }

    cursor = input;
    while (*cursor != '\0') {
        remaining = input_len - (size_t)(cursor - input);
        if (regexec(&regex, cursor, 1, &match, 0) == 0) {
            len = (size_t)match.rm_so;
            if (len > 0) {
                if (n >= capacity) {
                    capacity *= 2;
                    temp = realloc(result, capacity * sizeof(*result));
                    if (temp == NULL) {
                        goto cleanup;
                    }
                    result = temp;
                }
                buf_size = len + 1;
                result[n] = malloc(buf_size);
                if (result[n] == NULL) {
                    goto cleanup;
                }
                if (len <= remaining && buf_size > len) {
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(result[n], cursor, len);
                    result[n][len] = '\0';
                    n++;
                } else {
                    free(result[n]);
                    result[n] = NULL;
                    goto cleanup;
                }
            }
            cursor += match.rm_so;
            start = 0;
            while (*cursor != '\0' && regexec(&regex, cursor, 1, &match, 0) == 0 && match.rm_so == 0) {
                if (start > 0) {
                    if (n >= capacity) {
                        capacity *= 2;
                        temp = realloc(result, capacity * sizeof(*result));
                        if (temp == NULL) {
                            goto cleanup;
                        }
                        result = temp;
                    }
                    result[n] = malloc(2);
                    if (result[n] == NULL) {
                        goto cleanup;
                    }
                    result[n][0] = *cursor;
                    result[n][1] = '\0';
                    n++;
                }
                cursor++;
                start++;
            }
        } else {
            len = remaining;
            if (len > 0) {
                if (n >= capacity) {
                    capacity *= 2;
                    temp = realloc(result, capacity * sizeof(*result));
                    if (temp == NULL) {
                        goto cleanup;
                    }
                    result = temp;
                }
                buf_size = len + 1;
                result[n] = malloc(buf_size);
                if (result[n] == NULL) {
                    goto cleanup;
                }
                if (len <= remaining && buf_size > len) {
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(result[n], cursor, len);
                    result[n][len] = '\0';
                    n++;
                } else {
                    free(result[n]);
                    result[n] = NULL;
                    goto cleanup;
                }
            }
            break;
        }
    }

    regfree(&regex);
    *tokens = result;
    *count = n;
    return 0;

cleanup:
    for (size_t i = 0; i < n; i++) {
        free(result[i]);
    }
    free(result);
    regfree(&regex);
    return -1;
}

int main(void)
{
    const char *input = "SplitThisStringAtUppercaseLetters";
    char **tokens = NULL;
    size_t count = 0;

    if (split_at_uppercase(input, &tokens, &count) != 0) {
        fprintf(stderr, "Error splitting string\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; i++) {
        printf("%s\n", tokens[i]);
        free(tokens[i]);
    }
    free(tokens);

    return EXIT_SUCCESS;
}