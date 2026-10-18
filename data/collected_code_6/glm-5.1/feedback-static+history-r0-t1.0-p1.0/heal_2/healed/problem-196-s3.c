#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *elements;
    int length;
} Tuple;

typedef struct {
    Tuple *data;
    int count;
    int capacity;
} TupleList;

TupleList* create_tuple_list(int capacity) {
    TupleList *list = (TupleList*)malloc(sizeof(TupleList));
    if (!list) return NULL;
    list->data = (Tuple*)malloc(capacity * sizeof(Tuple));
    if (!list->data) {
        free(list);
        return NULL;
    }
    list->count = 0;
    list->capacity = capacity;
    return list;
}

int insert_tuple(TupleList *list, const int *elements, int length) {
    if (!list || list->count >= list->capacity) return 0;
    list->data[list->count].elements = (int*)malloc(length * sizeof(int));
    if (!list->data[list->count].elements) return 0;
    for (int i = 0; i < length; i++) {
        list->data[list->count].elements[i] = elements[i];
    }
    list->data[list->count].length = length;
    list->count++;
    return 1;
}

void remove_tuples_of_length_k(TupleList *list, int k) {
    if (!list) return;
    int new_count = 0;
    for (int i = 0; i < list->count; i++) {
        if (list->data[i].length == k) {
            free(list->data[i].elements);
            list->data[i].elements = NULL;
        } else {
            if (new_count != i) {
                list->data[new_count] = list->data[i];
                list->data[i].elements = NULL;
            }
            new_count++;
        }
    }
    list->count = new_count;
}

void free_tuple_list(TupleList *list) {
    if (!list) return;
    for (int i = 0; i < list->count; i++) {
        free(list->data[i].elements);
    }
    free(list->data);
    free(list);
}

void print_tuple_list(const TupleList *list) {
    if (!list) return;
    for (int i = 0; i < list->count; i++) {
        printf("(");
        for (int j = 0; j < list->data[i].length; j++) {
            printf("%d", list->data[i].elements[j]);
            if (j < list->data[i].length - 1) printf(", ");
        }
        printf(")\n");
    }
}

int main() {
    TupleList *list = create_tuple_list(10);
    if (!list) return 1;

    const int a1[] = {1, 2, 3};
    const int a2[] = {4, 5};
    const int a3[] = {6, 7, 8, 9};
    const int a4[] = {10, 11};
    const int a5[] = {12};

    insert_tuple(list, a1, 3);
    insert_tuple(list, a2, 2);
    insert_tuple(list, a3, 4);
    insert_tuple(list, a4, 2);
    insert_tuple(list, a5, 1);

    printf("Original list:\n");
    print_tuple_list(list);

    remove_tuples_of_length_k(list, 2);

    printf("\nAfter removing tuples of length 2:\n");
    print_tuple_list(list);

    free_tuple_list(list);

    return 0;
}