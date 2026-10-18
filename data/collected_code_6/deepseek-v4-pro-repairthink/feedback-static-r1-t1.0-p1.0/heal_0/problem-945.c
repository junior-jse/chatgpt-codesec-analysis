#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *elements;
    size_t size;
    size_t capacity;
} Set;

typedef struct {
    int *data;
    size_t length;
} Tuple;

void set_init(Set *set, size_t capacity) {
    set->elements = (int *)malloc(capacity * sizeof(int));
    if (set->elements == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    set->size = 0;
    set->capacity = capacity;
}

void set_free(Set *set) {
    free(set->elements);
    set->elements = NULL;
    set->size = 0;
    set->capacity = 0;
}

int set_contains(const Set *set, int value) {
    for (size_t i = 0; i < set->size; i++) {
        if (set->elements[i] == value) {
            return 1;
        }
    }
    return 0;
}

int set_add(Set *set, int value) {
    if (set_contains(set, value)) {
        return 0;
    }

    if (set->size == set->capacity) {
        size_t new_capacity = set->capacity * 2;
        int *new_elements = (int *)realloc(set->elements, new_capacity * sizeof(int));
        if (new_elements == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            return -1;
        }
        set->elements = new_elements;
        set->capacity = new_capacity;
    }

    set->elements[set->size] = value;
    set->size++;
    return 1;
}

Set tuples_to_set(const Tuple *tuples, size_t num_tuples) {
    Set result;
    set_init(&result, 10);

    for (size_t i = 0; i < num_tuples; i++) {
        for (size_t j = 0; j < tuples[i].length; j++) {
            if (set_add(&result, tuples[i].data[j]) == -1) {
                set_free(&result);
                exit(EXIT_FAILURE);
            }
        }
    }

    return result;
}

void print_set(const Set *set) {
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
    int data1[] = {1, 2, 3};
    int data2[] = {3, 4, 5};
    int data3[] = {5, 6, 7, 1};

    Tuple tuples[3];
    tuples[0].data = data1;
    tuples[0].length = sizeof(data1) / sizeof(data1[0]);
    tuples[1].data = data2;
    tuples[1].length = sizeof(data2) / sizeof(data2[0]);
    tuples[2].data = data3;
    tuples[2].length = sizeof(data3) / sizeof(data3[0]);

    Set result = tuples_to_set(tuples, 3);

    printf("Resulting set: ");
    print_set(&result);

    set_free(&result);

    return 0;
}