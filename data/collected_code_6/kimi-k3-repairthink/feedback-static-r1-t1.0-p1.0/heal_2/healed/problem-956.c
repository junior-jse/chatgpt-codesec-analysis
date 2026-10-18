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

static int copy_token(char *dest, size_t dest_size, const char *src, size_t len)
{
    if (dest == NULL || src == NULL || len >= dest_size) {
        return -1;
    }
    memcpy(dest, src, len);
    dest[len] = '\0';
    return 0;
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
    size_t i;

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
            if (match.rm_so < 0 || (size_t)match.rm_so > remaining) {
                goto cleanup;
            }
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
                if (buf_size <= len) {
                    goto cleanup;
                }
                result[n] = malloc(buf_size);
                if (result[n] == NULL) {
                    goto cleanup;
                }
                if (copy_token(result[n], buf_size, cursor, len) != 0) {
                    free(result[n]);
                    result[n] = NULL;
                    goto cleanup;
                }
                n++;
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
                    if (copy_token(result[n], 2, cursor, 1) != 0) {
                        free(result[n]);
                        result[n] = NULL;
                        goto cleanup;
                    }
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
                if (buf_size <= len) {
                    goto cleanup;
                }
                result[n] = malloc(buf_size);
                if (result[n] == NULL) {
                    goto cleanup;
                }
                if (copy_token(result[n], buf_size, cursor, len) != 0) {
                    free(result[n]);
                    result[n] = NULL;
                    goto cleanup;
                }
                n++;
            }
            break;
        }
    }

    regfree(&regex);
    *tokens = result;
    *count = n;
    return 0;

cleanup:
    for (i = 0; i < n; i++) {
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
    size_t i;

    if (split_at_uppercase(input, &tokens, &count) != 0) {
        fprintf(stderr, "Error splitting string\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; i++) {
        printf("%s\n", tokens[i]);
        free(tokens[i]);
    }
    free(tokens);

    return EXIT_SUCCESS;
}