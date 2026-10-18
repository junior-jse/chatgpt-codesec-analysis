#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *elements;
    size_t size;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t size;
    size_t capacity;
} TupleList;

TupleList *create_tuple_list(size_t initial_capacity) {
    TupleList *list = malloc(sizeof(TupleList));
    if (list == NULL) {
        return NULL;
    }
    
    list->tuples = malloc(initial_capacity * sizeof(Tuple));
    if (list->tuples == NULL) {
        free(list);
        return NULL;
    }
    
    list->size = 0;
    list->capacity = initial_capacity;
    return list;
}

int add_tuple(TupleList *list, const int *elements, size_t size) {
    if (list == NULL) {
        return 0;
    }
    
    if (list->size >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        Tuple *new_tuples = realloc(list->tuples, new_capacity * sizeof(Tuple));
        if (new_tuples == NULL) {
            return 0;
        }
        list->tuples = new_tuples;
        list->capacity = new_capacity;
    }
    
    Tuple *new_tuple = &list->tuples[list->size];
    new_tuple->size = size;
    
    if (size > 0) {
        new_tuple->elements = malloc(size * sizeof(int));
        if (new_tuple->elements == NULL) {
            return 0;
        }
        memcpy(new_tuple->elements, elements, size * sizeof(int));
    } else {
        new_tuple->elements = NULL;
    }
    
    list->size++;
    return 1;
}

void remove_empty_tuples(TupleList *list) {
    if (list == NULL) {
        return;
    }
    
    size_t write_index = 0;
    for (size_t read_index = 0; read_index < list->size; read_index++) {
        if (list->tuples[read_index].size > 0) {
            if (write_index != read_index) {
                list->tuples[write_index] = list->tuples[read_index];
            }
            write_index++;
        } else {
            free(list->tuples[read_index].elements);
        }
    }
    list->size = write_index;
}

void free_tuple_list(TupleList *list) {
    if (list == NULL) {
        return;
    }
    
    for (size_t i = 0; i < list->size; i++) {
        free(list->tuples[i].elements);
    }
    free(list->tuples);
    free(list);
}

void print_tuple_list(const TupleList *list) {
    if (list == NULL) {
        printf("List is NULL\n");
        return;
    }
    
    printf("[");
    for (size_t i = 0; i < list->size; i++) {
        printf("(");
        for (size_t j = 0; j < list->tuples[i].size; j++) {
            printf("%d", list->tuples[i].elements[j]);
            if (j < list->tuples[i].size - 1) {
                printf(", ");
            }
        }
        printf(")");
        if (i < list->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(void) {
    TupleList *list = create_tuple_list(4);
    if (list == NULL) {
        fprintf(stderr, "Failed to create tuple list\n");
        return EXIT_FAILURE;
    }
    
    int tuple1[] = {1, 2, 3};
    int tuple2[] = {4, 5};
    int tuple3[] = {6};
    int tuple4[] = {7, 8, 9, 10};
    
    if (!add_tuple(list, tuple1, 3)) {
        fprintf(stderr, "Failed to add tuple\n");
        free_tuple_list(list);
        return EXIT_FAILURE;
    }
    
    if (!add_tuple(list, NULL, 0)) {
        fprintf(stderr, "Failed to add tuple\n");
        free_tuple_list(list);
        return EXIT_FAILURE;
    }
    
    if (!add_tuple(list, tuple2, 2)) {
        fprintf(stderr, "Failed to add tuple\n");
        free_tuple_list(list);
        return EXIT_FAILURE;
    }
    
    if (!add_tuple(list, NULL, 0)) {
        fprintf(stderr, "Failed to add tuple\n");
        free_tuple_list(list);
        return EXIT_FAILURE;
    }
    
    if (!add_tuple(list, tuple3, 1)) {
        fprintf(stderr, "Failed to add tuple\n");
        free_tuple_list(list);
        return EXIT_FAILURE;
    }
    
    if (!add_tuple(list, tuple4, 4)) {
        fprintf(stderr, "Failed to add tuple\n");
        free_tuple_list(list);
        return EXIT_FAILURE;
    }
    
    printf("Before removing empty tuples:\n");
    print_tuple_list(list);
    
    remove_empty_tuples(list);
    
    printf("After removing empty tuples:\n");
    print_tuple_list(list);
    
    free_tuple_list(list);
    return EXIT_SUCCESS;
}