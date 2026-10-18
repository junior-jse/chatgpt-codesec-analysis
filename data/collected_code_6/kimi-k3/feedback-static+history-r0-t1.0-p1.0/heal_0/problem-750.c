#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} TupleList;

int init_list(TupleList *list, size_t initial_capacity) {
    if (list == NULL || initial_capacity == 0) {
        return -1;
    }
    list->data = malloc(initial_capacity * sizeof(int));
    if (list->data == NULL) {
        return -1;
    }
    list->size = 0;
    list->capacity = initial_capacity;
    return 0;
}

int add_tuple(TupleList *list, int first, int second) {
    if (list == NULL) {
        return -1;
    }
    if (list->size + 2 > list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity < list->size + 2) {
            new_capacity = list->size + 2;
        }
        int *new_data = realloc(list->data, new_capacity * sizeof(int));
        if (new_data == NULL) {
            return -1;
        }
        list->data = new_data;
        list->capacity = new_capacity;
    }
    list->data[list->size] = first;
    list->data[list->size + 1] = second;
    list->size += 2;
    return 0;
}

void free_list(TupleList *list) {
    if (list != NULL) {
        free(list->data);
        list->data = NULL;
        list->size = 0;
        list->capacity = 0;
    }
}

int main(void) {
    TupleList list;
    if (init_list(&list, 4) != 0) {
        fprintf(stderr, "Failed to initialize list\n");
        return EXIT_FAILURE;
    }

    if (add_tuple(&list, 10, 20) != 0) {
        fprintf(stderr, "Failed to add tuple\n");
        free_list(&list);
        return EXIT_FAILURE;
    }

    if (add_tuple(&list, 30, 40) != 0) {
        fprintf(stderr, "Failed to add tuple\n");
        free_list(&list);
        return EXIT_FAILURE;
    }

    printf("List contents: ");
    for (size_t i = 0; i < list.size; i++) {
        printf("%d ", list.data[i]);
    }
    printf("\n");

    free_list(&list);
    return EXIT_SUCCESS;
}