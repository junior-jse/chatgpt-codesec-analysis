#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **string_to_list(const char *str, const char *delim, size_t *count) {
    size_t capacity = 10;
    size_t size = 0;
    char **list = malloc(capacity * sizeof(char *));
    if (!list) {
        return NULL;
    }

    char *temp = strdup(str);
    if (!temp) {
        free(list);
        return NULL;
    }

    char *token = strtok(temp, delim);
    while (token != NULL) {
        if (size >= capacity) {
            capacity *= 2;
            char **new_list = realloc(list, capacity * sizeof(char *));
            if (!new_list) {
                for (size_t i = 0; i < size; i++) {
                    free(list[i]);
                }
                free(list);
                free(temp);
                return NULL;
            }
            list = new_list;
        }

        list[size] = strdup(token);
        if (!list[size]) {
            for (size_t i = 0; i < size; i++) {
                free(list[i]);
            }
            free(list[size]);
            free(list);
            free(temp);
            return NULL;
        }

        size++;
        token = strtok(NULL, delim);
    }

    free(temp);

    char **final_list = realloc(list, (size + 1) * sizeof(char *));
    if (!final_list) {
        for (size_t i = 0; i < size; i++) {
            free(list[i]);
        }
        free(list);
        return NULL;
    }
    list = final_list;
    list[size] = NULL;

    *count = size;
    return list;
}

void free_list(char **list) {
    if (!list) {
        return;
    }
    for (size_t i = 0; list[i] != NULL; i++) {
        free(list[i]);
    }
    free(list);
}

int main(void) {
    const char *input = "hello,world,this,is,a,test";
    const char *delim = ",";
    size_t count = 0;

    char **list = string_to_list(input, delim, &count);
    if (!list) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; i++) {
        printf("%s\n", list[i]);
    }

    free_list(list);

    return EXIT_SUCCESS;
}