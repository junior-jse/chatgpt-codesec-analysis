#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct {
    Tuple *items;
    size_t size;
    size_t capacity;
} TupleSet;

void init_set(TupleSet *set);
void add_tuple(TupleSet *set, int first, int second);
int contains_tuple(TupleSet *set, int first, int second);
int compare_tuples(const void *a, const void *b);
TupleSet convert_to_set(Tuple *tuples, size_t count);
void free_set(TupleSet *set);
void print_set(TupleSet *set);

void init_set(TupleSet *set) {
    if (set == NULL) {
        return;
    }
    set->items = NULL;
    set->size = 0;
    set->capacity = 0;
}

void add_tuple(TupleSet *set, int first, int second) {
    /* Possible weaknesses found:
     *  The scope of the variable 'new_capacity' can be reduced. [variableScope]
     */
    size_t new_capacity;
    Tuple *new_items;

    if (set == NULL) {
        return;
    }

    if (contains_tuple(set, first, second)) {
        return;
    }

    if (set->size == set->capacity) {
        new_capacity = set->capacity == 0 ? 1 : set->capacity * 2;
        new_items = (Tuple *)realloc(set->items, new_capacity * sizeof(Tuple));
        if (new_items == NULL) {
            return;
        }
        set->items = new_items;
        set->capacity = new_capacity;
    }

    set->items[set->size].first = first;
    set->items[set->size].second = second;
    set->size++;
}

int contains_tuple(TupleSet *set, int first, int second) {
    size_t i;

    if (set == NULL || set->items == NULL) {
        return 0;
    }

    for (i = 0; i < set->size; i++) {
        if (set->items[i].first == first && set->items[i].second == second) {
            return 1;
        }
    }

    return 0;
}

int compare_tuples(const void *a, const void *b) {
    const Tuple *tuple_a = (const Tuple *)a;
    const Tuple *tuple_b = (const Tuple *)b;
    
    if (tuple_a->first != tuple_b->first) {
        return tuple_a->first > tuple_b->first ? 1 : -1;
    }
    
    if (tuple_a->second != tuple_b->second) {
        return tuple_a->second > tuple_b->second ? 1 : -1;
    }
    
    return 0;
}

TupleSet convert_to_set(Tuple *tuples, size_t count) {
    TupleSet set;
    size_t i;

    init_set(&set);

    if (tuples == NULL || count == 0) {
        return set;
    }

    for (i = 0; i < count; i++) {
        add_tuple(&set, tuples[i].first, tuples[i].second);
    }

    if (set.size > 1) {
        qsort(set.items, set.size, sizeof(Tuple), compare_tuples);
    }

    return set;
}

void free_set(TupleSet *set) {
    if (set == NULL) {
        return;
    }

    if (set->items != NULL) {
        free(set->items);
        set->items = NULL;
    }
    set->size = 0;
    set->capacity = 0;
}

void print_set(TupleSet *set) {
    size_t i;

    if (set == NULL || set->items == NULL) {
        printf("{}\n");
        return;
    }

    printf("{");
    for (i = 0; i < set->size; i++) {
        printf("(%d, %d)", set->items[i].first, set->items[i].second);
        if (i < set->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

int main(void) {
    Tuple tuples[] = {
        {1, 2},
        {2, 3},
        {1, 2},
        {3, 4},
        {2, 3},
        {5, 6},
        {1, 2}
    };
    size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    TupleSet set;

    set = convert_to_set(tuples, tuple_count);
    print_set(&set);
    free_set(&set);

    return 0;
}