#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct {
    Tuple *data;
    size_t size;
    size_t capacity;
} TupleList;

int init_list(TupleList *list, size_t initial_capacity) {
    if (list == NULL || initial_capacity == 0) {
        return -1;
    }
    list->data = malloc(initial_capacity * sizeof(Tuple));
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
    if (list->size >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        Tuple *new_data = realloc(list->data, new_capacity * sizeof(Tuple));
        if (new_data == NULL) {
            return -1;
        }
        list->data = new_data;
        list->capacity = new_capacity;
    }
    list->data[list->size].first = first;
    list->data[list->size].second = second;
    list->size++;
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
    
    if (add_tuple(&list, 1, 2) != 0) {
        fprintf(stderr, "Failed to add tuple\n");
        free_list(&list);
        return EXIT_FAILURE;
    }
    
    if (add_tuple(&list, 3, 4) != 0) {
        fprintf(stderr, "Failed to add tuple\n");
        free_list(&list);
        return EXIT_FAILURE;
    }
    
    if (add_tuple(&list, 5, 6) != 0) {
        fprintf(stderr, "Failed to add tuple\n");
        free_list(&list);
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < list.size; i++) {
        printf("(%d, %d)\n", list.data[i].first, list.data[i].second);
    }
    
    free_list(&list);
    return EXIT_SUCCESS;
}