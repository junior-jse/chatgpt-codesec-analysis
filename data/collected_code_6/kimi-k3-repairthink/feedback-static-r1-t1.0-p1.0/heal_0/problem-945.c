#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *elements;
    size_t size;
    size_t capacity;
} IntSet;

typedef struct {
    int *data;
    size_t size;
} IntTuple;

int set_init(IntSet *set, size_t initial_capacity) {
    if (set == NULL || initial_capacity == 0) {
        return -1;
    }
    set->elements = malloc(initial_capacity * sizeof(int));
    if (set->elements == NULL) {
        return -1;
    }
    set->size = 0;
    set->capacity = initial_capacity;
    return 0;
}

void set_free(IntSet *set) {
    if (set != NULL) {
        free(set->elements);
        set->elements = NULL;
        set->size = 0;
        set->capacity = 0;
    }
}

int set_contains(const IntSet *set, int value) {
    if (set == NULL || set->elements == NULL) {
        return 0;
    }
    for (size_t i = 0; i < set->size; i++) {
        if (set->elements[i] == value) {
            return 1;
        }
    }
    return 0;
}

int set_add(IntSet *set, int value) {
    if (set == NULL || set->elements == NULL) {
        return -1;
    }
    
    if (set_contains(set, value)) {
        return 0;
    }
    
    if (set->size >= set->capacity) {
        size_t new_capacity = set->capacity * 2;
        int *new_elements = realloc(set->elements, new_capacity * sizeof(int));
        if (new_elements == NULL) {
            return -1;
        }
        set->elements = new_elements;
        set->capacity = new_capacity;
    }
    
    set->elements[set->size] = value;
    set->size++;
    return 0;
}

int tuple_to_set(const IntTuple *tuple, IntSet *set) {
    if (tuple == NULL || set == NULL || tuple->data == NULL) {
        return -1;
    }
    
    if (set_init(set, tuple->size > 0 ? tuple->size : 1) != 0) {
        return -1;
    }
    
    for (size_t i = 0; i < tuple->size; i++) {
        if (set_add(set, tuple->data[i]) != 0) {
            set_free(set);
            return -1;
        }
    }
    
    return 0;
}

void print_set(const IntSet *set) {
    if (set == NULL || set->elements == NULL) {
        printf("{}\n");
        return;
    }
    
    printf("{");
    for (size_t i = 0; i < set->size; i++) {
        printf("%d", set->elements[i]);
        if (i < set->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

int main(void) {
    int tuple_data[] = {1, 2, 3, 2, 4, 5, 1, 6, 3};
    IntTuple tuple = {tuple_data, sizeof(tuple_data) / sizeof(tuple_data[0])};
    IntSet result_set;
    
    printf("Original tuple: ");
    printf("(");
    for (size_t i = 0; i < tuple.size; i++) {
        printf("%d", tuple.data[i]);
        if (i < tuple.size - 1) {
            printf(", ");
        }
    }
    printf(")\n");
    
    if (tuple_to_set(&tuple, &result_set) != 0) {
        fprintf(stderr, "Error: Failed to convert tuple to set\n");
        return EXIT_FAILURE;
    }
    
    printf("Converted set: ");
    print_set(&result_set);
    
    set_free(&result_set);
    
    return EXIT_SUCCESS;
}