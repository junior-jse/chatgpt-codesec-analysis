#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STR_LEN 4096

typedef struct {
    char *str;
    int *list;
    size_t list_size;
} Tuple;

Tuple *create_tuple(const char *str, const int *list, size_t list_size) {
    if (str == NULL || (list == NULL && list_size > 0)) {
        return NULL;
    }

    size_t str_len = strnlen(str, MAX_STR_LEN);
    if (str_len == MAX_STR_LEN) {
        return NULL;
    }

    if (list_size > 0 && list_size > SIZE_MAX / sizeof(int)) {
        return NULL;
    }

    Tuple *tuple = (Tuple *)malloc(sizeof(Tuple));
    if (tuple == NULL) {
        return NULL;
    }

    tuple->str = (char *)malloc(str_len + 1);
    if (tuple->str == NULL) {
        free(tuple);
        return NULL;
    }
    memcpy(tuple->str, str, str_len + 1);

    tuple->list_size = list_size;
    if (list_size > 0) {
        tuple->list = (int *)malloc(list_size * sizeof(int));
        if (tuple->list == NULL) {
            free(tuple->str);
            free(tuple);
            return NULL;
        }
        for (size_t i = 0; i < list_size; i++) {
            tuple->list[i] = list[i];
        }
    } else {
        tuple->list = NULL;
    }

    return tuple;
}

void free_tuple(Tuple *tuple) {
    if (tuple != NULL) {
        free(tuple->str);
        free(tuple->list);
        free(tuple);
    }
}

int main(void) {
    const char *str = "example";
    int list[] = {1, 2, 3, 4, 5};
    size_t list_size = sizeof(list) / sizeof(list[0]);

    Tuple *tuple = create_tuple(str, list, list_size);
    if (tuple == NULL) {
        fprintf(stderr, "Failed to create tuple\n");
        return EXIT_FAILURE;
    }

    printf("String: %s\n", tuple->str);
    printf("List: ");
    for (size_t i = 0; i < tuple->list_size; i++) {
        printf("%d ", tuple->list[i]);
    }
    printf("\n");

    free_tuple(tuple);

    return EXIT_SUCCESS;
}