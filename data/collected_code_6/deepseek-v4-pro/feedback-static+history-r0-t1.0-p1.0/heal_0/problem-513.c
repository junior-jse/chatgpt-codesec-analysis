#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *convert_tuple_to_list(const char *tuple_str, const char *suffix, int *out_len) {
    if (!tuple_str || !suffix || !out_len) {
        *out_len = 0;
        return NULL;
    }

    size_t tuple_len = strlen(tuple_str);
    if (tuple_len < 2 || tuple_str[0] != '(' || tuple_str[tuple_len - 1] != ')') {
        *out_len = 0;
        return NULL;
    }

    size_t content_len = tuple_len - 2;
    char *content = (char *)malloc(content_len + 1);
    if (!content) {
        *out_len = 0;
        return NULL;
    }
    memcpy(content, tuple_str + 1, content_len);
    content[content_len] = '\0';

    size_t suffix_len = strlen(suffix);
    size_t capacity = 64;
    size_t length = 0;
    char *result = (char *)malloc(capacity);
    if (!result) {
        free(content);
        *out_len = 0;
        return NULL;
    }
    result[0] = '\0';

    char *token = strtok(content, ",");
    int first = 1;

    while (token) {
        while (*token == ' ') token++;
        size_t token_len = strlen(token);
        while (token_len > 0 && token[token_len - 1] == ' ') {
            token[--token_len] = '\0';
        }

        size_t entry_len = token_len + suffix_len + 2;
        if (length + entry_len + 1 > capacity) {
            capacity *= 2;
            while (capacity < length + entry_len + 1) capacity *= 2;
            char *temp = (char *)realloc(result, capacity);
            if (!temp) {
                free(result);
                free(content);
                *out_len = 0;
                return NULL;
            }
            result = temp;
        }

        if (!first) {
            result[length++] = ' ';
        }
        memcpy(result + length, token, token_len);
        length += token_len;
        memcpy(result + length, suffix, suffix_len);
        length += suffix_len;
        result[length] = '\0';
        first = 0;

        token = strtok(NULL, ",");
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