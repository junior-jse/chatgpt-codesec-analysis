#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntSet;

typedef struct {
    int *data;
    size_t size;
} IntTuple;

static int int_compare(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

IntSet *set_create(void) {
    IntSet *set = malloc(sizeof(IntSet));
    if (set == NULL) {
        return NULL;
    }
    set->data = NULL;
    set->size = 0;
    set->capacity = 0;
    return set;
}

void set_destroy(IntSet *set) {
    if (set != NULL) {
        free(set->data);
        free(set);
    }
}

int set_add(IntSet *set, int value) {
    if (set == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < set->size; i++) {
        if (set->data[i] == value) {
            return 0;
        }
    }
    
    if (set->size >= set->capacity) {
        size_t new_capacity = (set->capacity == 0) ? 8 : set->capacity * 2;
        int *new_data = realloc(set->data, new_capacity * sizeof(int));
        if (new_data == NULL) {
            return -1;
        }
        set->data = new_data;
        set->capacity = new_capacity;
    }
    
    set->data[set->size] = value;
    set->size++;
    return 0;
}

IntSet *tuple_to_set(const IntTuple *tuple) {
    if (tuple == NULL || (tuple->data == NULL && tuple->size > 0)) {
        return NULL;
    }
    
    IntSet *set = set_create();
    if (set == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < tuple->size; i++) {
        if (set_add(set, tuple->data[i]) != 0) {
            set_destroy(set);
            return NULL;
        }
    }
    
    if (set->size > 1) {
        qsort(set->data, set->size, sizeof(int), int_compare);
    }
    
    return set;
}

IntTuple *tuple_create(const int *values, size_t size) {
    if (values == NULL && size > 0) {
        return NULL;
    }
    
    IntTuple *tuple = malloc(sizeof(IntTuple));
    if (tuple == NULL) {
        return NULL;
    }
    
    tuple->data = NULL;
    tuple->size = size;
    
    if (size > 0) {
        tuple->data = malloc(size * sizeof(int));
        if (tuple->data == NULL) {
            free(tuple);
            return NULL;
        }
        for (size_t i = 0; i < size; i++) {
            tuple->data[i] = values[i];
        }
    }
    
    return tuple;
}

void tuple_destroy(IntTuple *tuple) {
    if (tuple != NULL) {
        free(tuple->data);
        free(tuple);
    }
}

void set_print(const IntSet *set) {
    if (set == NULL) {
        printf("NULL\n");
        return;
    }
    printf("{");
    for (size_t i = 0; i < set->size; i++) {
        printf("%d", set->data[i]);
        if (i < set->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

int main(void) {
    const int values1[] = {1, 2, 3, 2, 4, 1, 5};
    const int values2[] = {7, 7, 7, 7};
    const int values3[] = {10, 20, 30};
    
    IntTuple *tuple1 = tuple_create(values1, 7);
    if (tuple1 == NULL) {
        fprintf(stderr, "Failed to create tuple1\n");
        return EXIT_FAILURE;
    }
    
    IntTuple *tuple2 = tuple_create(values2, 4);
    if (tuple2 == NULL) {
        fprintf(stderr, "Failed to create tuple2\n");
        tuple_destroy(tuple1);
        return EXIT_FAILURE;
    }
    
    IntTuple *tuple3 = tuple_create(values3, 3);
    if (tuple3 == NULL) {
        fprintf(stderr, "Failed to create tuple3\n");
        tuple_destroy(tuple1);
        tuple_destroy(tuple2);
        return EXIT_FAILURE;
    }
    
    IntSet *set1 = tuple_to_set(tuple1);
    IntSet *set2 = tuple_to_set(tuple2);
    IntSet *set3 = tuple_to_set(tuple3);
    
    if (set1 == NULL || set2 == NULL || set3 == NULL) {
        fprintf(stderr, "Failed to convert tuple to set\n");
        set_destroy(set1);
        set_destroy(set2);
        set_destroy(set3);
        tuple_destroy(tuple1);
        tuple_destroy(tuple2);
        tuple_destroy(tuple3);
        return EXIT_FAILURE;
    }
    
    printf("Tuple1 -> Set1: ");
    set_print(set1);
    
    printf("Tuple2 -> Set2: ");
    set_print(set2);
    
    printf("Tuple3 -> Set3: ");
    set_print(set3);
    
    set_destroy(set1);
    set_destroy(set2);
    set_destroy(set3);
    tuple_destroy(tuple1);
    tuple_destroy(tuple2);
    tuple_destroy(tuple3);
    
    return EXIT_SUCCESS;
}