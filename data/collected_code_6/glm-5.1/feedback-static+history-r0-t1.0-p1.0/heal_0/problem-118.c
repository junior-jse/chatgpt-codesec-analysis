#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **string_to_list(const char *str, const char *delim, size_t *len) {
    char **list = NULL;
    size_t capacity = 0;
    size_t size = 0;
    char *temp_str = NULL;
    char *token = NULL;
    char *saveptr = NULL;

    if (str == NULL || delim == NULL || len == NULL) {
        return NULL;
    }

    temp_str = strdup(str);
    if (temp_str == NULL) {
        return NULL;
    }

    capacity = 4;
    list = malloc(capacity * sizeof(char *));
    if (list == NULL) {
        free(temp_str);
        return NULL;
    }

    token = strtok_r(temp_str, delim, &saveptr);
    while (token != NULL) {
        if (size >= capacity) {
            capacity *= 2;
            char **new_list = realloc(list, capacity * sizeof(char *));
            if (new_list == NULL) {
                for (size_t i = 0; i < size; i++) {
                    free(list[i]);
                }
                free(list);
                free(temp_str);
                return NULL;
            }
            list = new_list;
        }

        list[size] = strdup(token);
        if (list[size] == NULL) {
            for (size_t i = 0; i < size; i++) {
                free(list[i]);
            }
            free(list);
            free(temp_str);
            return NULL;
        }

        size++;
        token = strtok_r(NULL, delim, &saveptr);
    }

    free(temp_str);
    *len = size;

    char **final_list = realloc(list, (size + 1) * sizeof(char *));
    if (final_list != NULL) {
        final_list[size] = NULL;
        return final_list;
    }

    list[size] = NULL;
    return list;
}

void free_list(char **list, size_t len) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < len; i++) {
        free(list[i]);
    }
    free(list);
}

int main(void) {
    const char *input = "Hello,world,this,is,a,test";
    const char *delimiter = ",";
    size_t length = 0;

    char **list = string_to_list(input, delimiter, &length);
    if (list == NULL) {
        fprintf(stderr, "Failed to convert string to list\n");
        return 1;
    }

    for (size_t i = 0; i < length; i++) {
        printf("%s\n", list[i]);
    }

    free_list(list, length);

    return 0;
}