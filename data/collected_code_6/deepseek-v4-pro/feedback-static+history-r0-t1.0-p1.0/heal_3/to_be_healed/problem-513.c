#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *convert_tuple_to_list(const char *tuple_str, const char *suffix, int *out_len) {
    if (!tuple_str || !suffix || !out_len) {
        return NULL;
    }

    size_t tuple_len = strnlen(tuple_str, SIZE_MAX);
    if (tuple_len < 2 || tuple_str[0] != '(' || tuple_str[tuple_len - 1] != ')') {
        return NULL;
    }

    size_t content_len = tuple_len - 2;
    char *content = (char *)malloc(content_len + 1);
    if (!content) {
        return NULL;
    }
    
    if (content_len > 0) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(content, tuple_str + 1, content_len);
    }
    content[content_len] = '\0';

    size_t suffix_len = strnlen(suffix, SIZE_MAX);
    size_t capacity = 64;
    size_t length = 0;
    char *result = (char *)malloc(capacity);
    if (!result) {
        free(content);
        return NULL;
    }
    result[0] = '\0';

    char *saveptr;
    char *token = strtok_r(content, ",", &saveptr);
    int first = 1;

    while (token) {
        while (*token == ' ') token++;
        size_t token_len = strnlen(token, SIZE_MAX);
        while (token_len > 0 && token[token_len - 1] == ' ') {
            token[--token_len] = '\0';
        }

        size_t entry_len = token_len + suffix_len + 2;
        if (length + entry_len + 1 > capacity) {
            size_t new_capacity = capacity * 2;
            while (new_capacity < length + entry_len + 1) {
                if (new_capacity > SIZE_MAX / 2) {
                    free(result);
                    free(content);
                    return NULL;
                }
                new_capacity *= 2;
            }
            char *temp = (char *)realloc(result, new_capacity);
            if (!temp) {
                free(result);
                free(content);
                return NULL;
            }
            result = temp;
            capacity = new_capacity;
        }

        if (!first) {
            result[length++] = ' ';
        }
        
        if (token_len > 0 && length + token_len <= capacity) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + length, token, token_len);
            length += token_len;
        }
        
        if (suffix_len > 0 && length + suffix_len <= capacity) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + length, suffix, suffix_len);
            length += suffix_len;
        }
        
        result[length] = '\0';
        first = 0;

        token = strtok_r(NULL, ",", &saveptr);
    }

    free(content);
    *out_len = (int)length;
    return result;
}

int main(void) {
    const char *tuple_str = "(apple, banana, cherry)";
    const char *suffix = "!";
    int len = 0;
    char *list = convert_tuple_to_list(tuple_str, suffix, &len);

    if (list) {
        printf("[%s]\n", list);
        printf("Length: %d\n", len);
        free(list);
    } else {
        printf("Invalid input or memory allocation failed.\n");
    }

    return 0;
}