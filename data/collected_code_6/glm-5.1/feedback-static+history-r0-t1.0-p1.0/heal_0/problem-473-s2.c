#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *elements;
    int count;
} Tuple;

typedef struct {
    Tuple *items;
    int count;
} TupleList;

int compare_ints(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

int tuples_equal(Tuple t1, Tuple t2) {
    if (t1.count != t2.count) return 0;
    for (int i = 0; i < t1.count; i++) {
        if (t1.elements[i] != t2.elements[i]) return 0;
    }
    return 1;
}

TupleList find_intersection(TupleList list) {
    TupleList result;
    result.items = NULL;
    result.count = 0;

    if (list.count == 0) return result;

    for (int i = 0; i < list.count; i++) {
        int *sorted = malloc(list.items[i].count * sizeof(int));
        if (!sorted && list.items[i].count > 0) exit(EXIT_FAILURE);
        memcpy(sorted, list.items[i].elements, list.items[i].count * sizeof(int));
        qsort(sorted, list.items[i].count, sizeof(int), compare_ints);

        int is_duplicate = 0;
        for (int j = 0; j < result.count; j++) {
            Tuple temp;
            temp.elements = sorted;
            temp.count = list.items[i].count;
            if (tuples_equal(result.items[j], temp)) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            Tuple new_tuple;
            new_tuple.count = list.items[i].count;
            new_tuple.elements = sorted;
            
            Tuple *new_items = realloc(result.items, (result.count + 1) * sizeof(Tuple));
            if (!new_items) exit(EXIT_FAILURE);
            
            result.items = new_items;
            result.items[result.count] = new_tuple;
            result.count++;
        } else {
            free(sorted);
        }
    }

    return result;
}

void free_tuple_list(TupleList list) {
    for (int i = 0; i < list.count; i++) {
        free(list.items[i].elements);
    }
    free(list.items);
}

int main() {
    int a1[] = {3, 1, 2};
    int a2[] = {1, 2, 3};
    int a3[] = {4, 5};
    int a4[] = {5, 4};
    int a5[] = {1, 2, 3};
    
    Tuple tuples[] = {
        {a1, 3},
        {a2, 3},
        {a3, 2},
        {a4, 2},
        {a5, 3}
    };
    
    TupleList list = {tuples, 5};
    TupleList intersection = find_intersection(list);
    
    for (int i = 0; i < intersection.count; i++) {
        printf("(");
        for (int j = 0; j < intersection.items[i].count; j++) {
            printf("%d", intersection.items[i].elements[j]);
            if (j < intersection.items[i].count - 1) printf(", ");
        }
        printf(")\n");
    }
    
    free_tuple_list(intersection);
    
    return 0;
}