#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char *value;
} Tuple;

typedef struct {
    int *ids;
    int size;
} OrderList;

int find_index(OrderList *order, int id) {
    for (int i = 0; i < order->size; i++) {
        if (order->ids[i] == id) {
            return i;
        }
    }
    return -1;
}

int compare_tuples(const void *a, const void *b, void *arg) {
    OrderList *order = (OrderList *)arg;
    const Tuple *tuple_a = (const Tuple *)a;
    const Tuple *tuple_b = (const Tuple *)b;
    
    int index_a = find_index(order, tuple_a->id);
    int index_b = find_index(order, tuple_b->id);
    
    if (index_a == -1 && index_b == -1) return 0;
    if (index_a == -1) return 1;
    if (index_b == -1) return -1;
    
    return index_a - index_b;
}

void rearrange_tuples(Tuple *tuples, int tuple_count, OrderList *order) {
    if (!tuples || !order || tuple_count <= 0 || order->size <= 0) {
        return;
    }
    
    /* Possible weaknesses found:
     *  passing argument 5 of 'qsort_r' from incompatible pointer type [-Wincompatible-pointer-types]
     */
    qsort_r(tuples, tuple_count, sizeof(Tuple), compare_tuples, order);
}

int main(void) {
    OrderList order;
    int ids_array[] = {3, 1, 2, 5};
    order.ids = ids_array;
    order.size = 4;
    
    Tuple tuples[4];
    tuples[0].id = 2;
    tuples[0].value = "two";
    tuples[1].id = 1;
    tuples[1].value = "one";
    tuples[2].id = 5;
    tuples[2].value = "five";
    tuples[3].id = 3;
    tuples[3].value = "three";
    
    rearrange_tuples(tuples, 4, &order);
    
    for (int i = 0; i < 4; i++) {
        printf("(%d, %s) ", tuples[i].id, tuples[i].value);
    }
    printf("\n");
    
    return 0;
}