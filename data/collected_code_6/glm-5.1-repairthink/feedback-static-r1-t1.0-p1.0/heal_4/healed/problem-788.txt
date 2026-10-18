#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    char *string;
    int *list;
    size_t list_size;
} Tuple;

Tuple *create_tuple(const char *str, size_t str_len, const int *list, size_t list_size) {
    if (str == NULL || (list == NULL && list_size > 0)) {
        return NULL;
    }

    Tuple *new_tuple = malloc(sizeof(Tuple));
    if (new_tuple == NULL) {
        return NULL;
    }

    if (str_len == SIZE_MAX) {
        free(new_tuple);
        return NULL;
    }

    size_t str_bytes = str_len + 1;
    new_tuple->string = malloc(str_bytes);
    if (new_tuple->string == NULL) {
        free(new_tuple);
        return NULL;
    }
    memcpy(new_tuple->string, str, str_len);
    new_tuple->string[str_len] = '\0';

    if (list_size > 0) {
        if (list_size > SIZE_MAX / sizeof(int)) {
            free(new_tuple->string);
            free(new_tuple);
            return NULL;
        }
        size_t list_bytes = list_size * sizeof(int);
        new_tuple->list = malloc(list_bytes);
        if (new_tuple->list == NULL) {
            free(new_tuple->string);
            free(new_tuple);
            return NULL;
        }
        memcpy(new_tuple->list, list, list_bytes);
    } else {
        new_tuple->list = NULL;
    }

    new_tuple->list_size = list_size;
    return new_tuple;
}

void free_tuple(Tuple *tuple) {
    if (tuple != NULL) {
        free(tuple->string);
        free(tuple->list);
        free(tuple);
    }
}

int main(void) {
    const char *str = "example";
    size_t str_len = strlen(str);
    int list[] = {1, 2, 3, 4, 5};
    size_t list_size = sizeof(list) / sizeof(list[0]);

    Tuple *my_tuple = create_tuple(str, str_len, list, list_size);
    if (my_tuple != NULL) {
        printf("String: %s\n", my_tuple->string);
        printf("List: ");
        for (size_t i = 0; i < my_tuple->list_size; i++) {
            printf("%d ", my_tuple->list[i]);
        }
        printf("\n");
        free_tuple(my_tuple);
    } else {
        fprintf(stderr, "Failed to create tuple\n");
        return 1;
    }

    return 0;
}