#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

char **split_string(const char *str, const char *pattern, int *count) {
    regex_t regex;
    regmatch_t match;
    char **result = NULL;
    char **temp = NULL;
    const char *cursor = str;
    int capacity = 10;
    int size = 0;
    int start = 0;
    int end = 0;

    if (str == NULL || pattern == NULL || count == NULL) {
        return NULL;
    }

    *count = 0;

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        return NULL;
    }

    result = malloc(capacity * sizeof(char *));
    if (result == NULL) {
        regfree(&regex);
        return NULL;
    }

    while (*cursor != '\0') {
        if (regexec(&regex, cursor, 1, &match, 0) == 0) {
            end = start + match.rm_so;
            
            if (end > start) {
                if (size >= capacity) {
                    capacity *= 2;
                    temp = realloc(result, capacity * sizeof(char *));
                    if (temp == NULL) {
                        for (int i = 0; i < size; i++) {
                            free(result[i]);
                        }
                        free(result);
                        regfree(&regex);
                        return NULL;
                    }
                    result = temp;
                }

                result[size] = malloc(end - start + 1);
                if (result[size] == NULL) {
                    for (int i = 0; i < size; i++) {
                        free(result[i]);
                    }
                    free(result);
                    regfree(&regex);
                    return NULL;
                }

                strncpy(result[size], str + start, end - start);
                result[size][end - start] = '\0';
                size++;
            }

            start = start + match.rm_eo;
            cursor = str + start;
        } else {
            break;
        }
    }

    if (strlen(cursor) > 0) {
        if (size >= capacity) {
            capacity++;
            temp = realloc(result, capacity * sizeof(char *));
            if (temp == NULL) {
                for (int i = 0; i < size; i++) {
                    free(result[i]);
                }
                free(result);
                regfree(&regex);
                return NULL;
            }
            result = temp;
        }

        result[size] = malloc(strlen(cursor) + 1);
        if (result[size] == NULL) {
            for (int i = 0; i < size; i++) {
                free(result[i]);
            }
            free(result);
            regfree(&regex);
            return NULL;
        }

        strcpy(result[size], cursor);
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