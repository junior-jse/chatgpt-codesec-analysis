#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *values;
    int size;
} TupleList;

int tuple_list_trim(TupleList *list, int k) {
    if (list == NULL || list->values == NULL) {
        return -1;
    }

    if (k <= 0) {
        return 0;
    }

    if (2 * k >= list->size) {
        free(list->values);
        list->values = NULL;
        list->size = 0;
        return 0;
    }

    int new_size = list->size - (2 * k);
    int *new_values = malloc(new_size * sizeof(int));
    if (new_values == NULL) {
        return -1;
    }

    for (int i = 0; i < new_size; i++) {
        new_values[i] = list->values[k + i];
    }

    free(list->values);
    list->values = new_values;
    list->size = new_size;

    return 0;
}

int main(void) {
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8};
    TupleList list = {data, 8};

    int k = 2;
    
    TupleList list_copy;
    list_copy.size = list.size;
    list_copy.values = malloc(list.size * sizeof(int));
    if (list_copy.values == NULL) {
        return 1;
    }
    for (int i = 0; i < list.size; i++) {
        list_copy.values[i] = list.values[i];
    }

    if (tuple_list_trim(&list_copy, k) == 0) {
        for (int i = 0; i < list_copy.size; i++) {
            printf("%d ", list_copy.values[i]);
        }
        printf("\n");
    }

    free(list_copy.values);

    return 0;
}