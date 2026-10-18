#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
} Tuple;

typedef struct {
    Tuple *tuples;
    int count;
} TupleList;

void trim_tuple(Tuple *tuple, int k) {
    if (tuple == NULL || tuple->size <= k || k < 0) {
        return;
    }
    int new_size = tuple->size - k;
    int *new_data = (int *)malloc(new_size * sizeof(int));
    if (new_data == NULL) {
        return;
    }
    for (int i = 0; i < new_size; i++) {
        new_data[i] = tuple->data[i];
    }
    free(tuple->data);
    tuple->data = new_data;
    tuple->size = new_size;
}

void trim_tuple_list(TupleList *list, int k) {
    if (list == NULL) {
        return;
    }
    for (int i = 0; i < list->count; i++) {
        trim_tuple(&(list->tuples[i]), k);
    }
}

TupleList create_tuple_list(int count) {
    TupleList list;
    list.count = count;
    list.tuples = (Tuple *)malloc(count * sizeof(Tuple));
    if (list.tuples == NULL) {
        list.count = 0;
        return list;
    }
    for (int i = 0; i < count; i++) {
        list.tuples[i].data = NULL;
        list.tuples[i].size = 0;
    }
    return list;
}

void init_tuple(TupleList *list, int index, const int *data, int size) {
    if (list == NULL || index < 0 || index >= list->count || data == NULL || size <= 0) {
        return;
    }
    list->tuples[index].data = (int *)malloc(size * sizeof(int));
    if (list->tuples[index].data == NULL) {
        list->tuples[index].size = 0;
        return;
    }
    for (int i = 0; i < size; i++) {
        list->tuples[index].data[i] = data[i];
    }
    list->tuples[index].size = size;
}

void free_tuple_list(TupleList *list) {
    if (list == NULL) {
        return;
    }
    for (int i = 0; i < list->count; i++) {
        if (list->tuples[i].data != NULL) {
            free(list->tuples[i].data);
            list->tuples[i].data = NULL;
        }
    }
    free(list->tuples);
    list->tuples = NULL;
    list->count = 0;
}

void print_tuple_list(TupleList *list) {
    if (list == NULL) {
        return;
    }
    for (int i = 0; i < list->count; i++) {
        printf("(");
        for (int j = 0; j < list->tuples[i].size; j++) {
            if (j > 0) {
                printf(", ");
            }
            printf("%d", list->tuples[i].data[j]);
        }
        printf(")\n");
    }
}

int main(void) {
    int data1[] = {1, 2, 3, 4, 5};
    int data2[] = {10, 20, 30};
    int data3[] = {7, 8, 9, 10, 11, 12};

    TupleList list = create_tuple_list(3);
    if (list.tuples == NULL) {
        return 1;
    }

    init_tuple(&list, 0, data1, 5);
    init_tuple(&list, 1, data2, 3);
    init_tuple(&list, 2, data3, 6);

    printf("Original tuple list:\n");
    print_tuple_list(&list);

    trim_tuple_list(&list, 2);

    printf("\nAfter trimming by 2:\n");
    print_tuple_list(&list);

    free_tuple_list(&list);

    return 0;
}