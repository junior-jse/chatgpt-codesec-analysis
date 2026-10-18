#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_INPUT_LENGTH 65536

static char *copy_token(const char *src, size_t len) {
    char *dest = (char *)malloc(len + 1);
    if (dest == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < len; i++) {
        dest[i] = src[i];
    }
    dest[len] = '\0';
    return dest;
}

static void free_tokens(char **tokens, int size) {
    for (int i = 0; i < size; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

char **split_string(const char *str, const char *pattern, int *count) {
    regex_t regex;
    regmatch_t match;
    char **result = NULL;
    char **temp = NULL;
    const char *cursor = str;
    int capacity = 10;
    int size = 0;
    size_t start = 0;
    size_t str_len;

    if (str == NULL || pattern == NULL || count == NULL) {
        return NULL;
    }

    *count = 0;

    str_len = strnlen(str, MAX_INPUT_LENGTH);
    if (str_len >= MAX_INPUT_LENGTH) {
        return NULL;
    }

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        return NULL;
    }

    result = (char **)malloc((size_t)capacity * sizeof(char *));
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    while (*cursor != '\0') {
        if (regexec(&regex, cursor, 1, &match, 0) == 0) {
            size_t end;

            if (match.rm_so < 0 || match.rm_eo < match.rm_so) {
                break;
            }

            end = start + (size_t)match.rm_so;

            if (end > start) {
                if (size >= capacity) {
                    if (capacity > MAX_INPUT_LENGTH / 2) {
                        free_tokens(result, size);
                        regfree(&regex);
                        return NULL;
                    }
                    capacity *= 2;
                    temp = (char **)realloc(result, (size_t)capacity * sizeof(char *));
                    if (temp == NULL) {
                        free_tokens(result, size);
                        regfree(&regex);
                        return NULL;
                    }
                    result = temp;
                }

                result[size] = copy_token(str + start, end - start);
                if (result[size] == NULL) {
                    free_tokens(result, size);
                    regfree(&regex);
                    return NULL;
                }
                size++;
            }

            if (match.rm_eo == 0) {
                start++;
                cursor = str + start;
                continue;
            }

            start = start + (size_t)match.rm_eo;
            cursor = str + start;
        } else {
            break;
        }
    }

    if (start < str_len) {
        size_t remaining = str_len - start;

        if (size >= capacity) {
            capacity++;
            temp = (char **)realloc(result, (size_t)capacity * sizeof(char *));
            if (temp == NULL) {
                free_tokens(result, size);
                regfree(&regex);
                return NULL;
            }
            result = temp;
        }

        result[size] = copy_token(str + start, remaining);
        if (result[size] == NULL) {
            free_tokens(result, size);
            regfree(&regex);
            return NULL;
        }
        size++;
    }

    regfree(&regex);
    *count = size;
    return result;
}

void free_split_result(char **result, int count) {
    if (result != NULL) {
        for (int i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
    }
}

int main(void) {
    const char *input = "apple,banana;orange:grape|mango";
    const char *pattern = "[,;:|]";
    int count = 0;
    char **tokens = split_string(input, pattern, &count);

    if (tokens != NULL) {
        for (int i = 0; i < count; i++) {
            printf("%s\n", tokens[i]);
        }
        free_split_result(tokens, count);
    }

    return 0;
}