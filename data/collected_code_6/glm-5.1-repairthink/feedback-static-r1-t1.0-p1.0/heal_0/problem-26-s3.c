#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *elements;
    int len;
} Tuple;

typedef struct {
    Tuple *tuples;
    int count;
} TupleList;

bool has_all_k_elements(TupleList list, int k) {
    if (list.tuples == NULL) {
        return false;
    }
    for (int i = 0; i < list.count; i++) {
        if (list.tuples[i].elements == NULL) {
            return false;
        }
        if (list.tuples[i].len != k) {
            return false;
        }
    }
    return true;
}

int main(void) {
    int a1[] = {1, 2, 3};
    int a2[] = {4, 5, 6};
    int a3[] = {7, 8, 9};
    
    Tuple t1 = {a1, 3};
    Tuple t2 = {a2, 3};
    Tuple t3 = {a3, 3};
    
    Tuple tuples[] = {t1, t2, t3};
    TupleList list = {tuples, 3};
    
    if (has_all_k_elements(list, 3)) {
        printf("All tuples have 3 elements\n");
    } else {
        printf("Not all tuples have 3 elements\n");
    }
    
    int b1[] = {1, 2};
    int b2[] = {3, 4, 5};
    
    Tuple u1 = {b1, 2};
    Tuple u2 = {b2, 3};
    
    Tuple tuples2[] = {u1, u2};
    TupleList list2 = {tuples2, 2};
    
    if (has_all_k_elements(list2, 3)) {
        printf("All tuples have 3 elements\n");
    } else {
        printf("Not all tuples have 3 elements\n");
    }
    
    TupleList empty_list = {NULL, 0};
    if (has_all_k_elements(empty_list, 3)) {
        printf("All tuples have 3 elements\n");
    } else {
        printf("Not all tuples have 3 elements\n");
    }
    
    return 0;
}