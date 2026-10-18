#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int a;
    int b;
} Tuple;

typedef struct {
    Tuple *items;
    size_t size;
    size_t capacity;
} TupleSet;

size_t tuple_hash(Tuple t) {
    size_t h1 = (size_t)t.a * 2654435761U;
    size_t h2 = (size_t)t.b * 2654435769U;
    return h1 ^ (h2 * 2654435789U);
}

int tuple_equal(Tuple t1, Tuple t2) {
    return t1.a == t2.a && t1.b == t2.b;
}

int set_insert(TupleSet *set, Tuple t) {
    for (size_t i = 0; i < set->size; i++) {
        if (tuple_equal(set->items[i], t)) {
            return 0;
        }
    }
    if (set->size >= set->capacity) {
        size_t new_capacity = set->capacity == 0 ? 4 : set->capacity * 2;
        Tuple *new_items = (Tuple *)realloc(set->items, new_capacity * sizeof(Tuple));
        if (new_items == NULL) {
            return -1;
        }
        set->items = new_items;
        set->capacity = new_capacity;
    }
    set->items[set->size] = t;
    set->size++;
    return 1;
}

TupleSet *tuples_to_set(const Tuple *tuples, size_t tuple_count) {
    TupleSet *set = (TupleSet *)malloc(sizeof(TupleSet));
    if (set == NULL) {
        return NULL;
    }
    set->items = NULL;
    set->size = 0;
    set->capacity = 0;
    for (size_t i = 0; i < tuple_count; i++) {
        if (set_insert(set, tuples[i]) == -1) {
            free(set->items);
            free(set);
            return NULL;
        }
    }
    return set;
}

void free_set(TupleSet *set) {
    if (set != NULL) {
        free(set->items);
        free(set);
    }
}

int main(void) {
    Tuple input[] = {{1, 2}, {3, 4}, {1, 2}, {5, 6}, {3, 4}};
    size_t count = sizeof(input) / sizeof(input[0]);
    TupleSet *set = tuples_to_set(input, count);
    if (set == NULL) {
        return 1;
    }
    for (size_t i = 0; i < set->size; i++) {
        printf("(%d, %d)\n", set->items[i].a, set->items[i].b);
    }
    free_set(set);
    return 0;
}