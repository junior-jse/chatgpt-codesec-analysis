#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *elements;
    size_t size;
    size_t capacity;
} Set;

typedef struct {
    int first;
    int second;
} Tuple;

void init_set(Set *set) {
    set->elements = NULL;
    set->size = 0;
    set->capacity = 0;
}

int contains(Set *set, int value) {
    for (size_t i = 0; i < set->size; i++) {
        if (set->elements[i] == value) {
            return 1;
        }
    }
    return 0;
}

int add_to_set(Set *set, int value) {
    if (contains(set, value)) {
        return 0;
    }
    if (set->size == set->capacity) {
        size_t new_capacity = set->capacity == 0 ? 8 : set->capacity * 2;
        int *new_elements = (int *)realloc(set->elements, new_capacity * sizeof(int));
        if (new_elements == NULL) {
            return -1;
        }
        set->elements = new_elements;
        set->capacity = new_capacity;
    }
    set->elements[set->size++] = value;
    return 1;
}

void free_set(Set *set) {
    free(set->elements);
    init_set(set);
}

Set tuples_to_set(Tuple *tuples, size_t tuple_count) {
    Set result;
    init_set(&result);
    if (tuples == NULL && tuple_count > 0) {
        return result;
    }
    for (size_t i = 0; i < tuple_count; i++) {
        if (add_to_set(&result, tuples[i].first) == -1) {
            free_set(&result);
            init_set(&result);
            return result;
        }
        if (add_to_set(&result, tuples[i].second) == -1) {
            free_set(&result);
            init_set(&result);
            return result;
        }
    }
    return result;
}

void print_set(Set *set) {
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
    Tuple tuples[] = {
        {1, 2},
        {3, 4},
        {1, 5},
        {2, 6},
        {3, 3}
    };
    size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);

    Set result = tuples_to_set(tuples, tuple_count);
    if (result.size == 0 && result.elements == NULL && tuple_count > 0) {
        printf("Memory allocation failed\n");
        free_set(&result);
        return EXIT_FAILURE;
    }

    print_set(&result);
    free_set(&result);

    return EXIT_SUCCESS;
}