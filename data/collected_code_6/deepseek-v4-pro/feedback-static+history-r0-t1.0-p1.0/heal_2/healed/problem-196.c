#include <stdio.h>
#include <stdlib.h>

typedef struct Tuple {
    int *data;
    int length;
    struct Tuple *next;
} Tuple;

typedef struct {
    Tuple *head;
} TupleList;

TupleList* create_tuple_list(void) {
    TupleList *list = (TupleList*)malloc(sizeof(TupleList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    return list;
}

int add_tuple(TupleList *list, const int *data, int length) {
    if (list == NULL || data == NULL || length <= 0) {
        return 0;
    }
    
    Tuple *new_tuple = (Tuple*)malloc(sizeof(Tuple));
    if (new_tuple == NULL) {
        return 0;
    }
    
    new_tuple->data = (int*)malloc(length * sizeof(int));
    if (new_tuple->data == NULL) {
        free(new_tuple);
        return 0;
    }
    
    for (int i = 0; i < length; i++) {
        new_tuple->data[i] = data[i];
    }
    new_tuple->length = length;
    new_tuple->next = list->head;
    list->head = new_tuple;
    return 1;
}

void remove_tuples_with_length(TupleList *list, int k) {
    if (list == NULL || list->head == NULL || k <= 0) {
        return;
    }
    
    Tuple *current = list->head;
    Tuple *previous = NULL;
    
    while (current != NULL) {
        if (current->length == k) {
            Tuple *to_delete = current;
            if (previous == NULL) {
                list->head = current->next;
                current = list->head;
            } else {
                previous->next = current->next;
                current = current->next;
            }
            free(to_delete->data);
            free(to_delete);
        } else {
            previous = current;
            current = current->next;
        }
    }
}

void print_tuples(const TupleList *list) {
    if (list == NULL) {
        return;
    }
    
    Tuple *current = list->head;
    printf("Tuples:\n");
    while (current != NULL) {
        printf("Length %d: [", current->length);
        for (int i = 0; i < current->length; i++) {
            if (i > 0) {
                printf(", ");
            }
            printf("%d", current->data[i]);
        }
        printf("]\n");
        current = current->next;
    }
}

void free_tuple_list(TupleList *list) {
    if (list == NULL) {
        return;
    }
    
    Tuple *current = list->head;
    while (current != NULL) {
        Tuple *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}

int main(void) {
    TupleList *list = create_tuple_list();
    if (list == NULL) {
        fprintf(stderr, "Failed to create list\n");
        return 1;
    }
    
    const int data1[] = {1, 2, 3};
    const int data2[] = {4, 5};
    const int data3[] = {6, 7, 8, 9};
    const int data4[] = {10, 11};
    const int data5[] = {12};
    
    add_tuple(list, data1, 3);
    add_tuple(list, data2, 2);
    add_tuple(list, data3, 4);
    add_tuple(list, data4, 2);
    add_tuple(list, data5, 1);
    
    printf("Before removal:\n");
    print_tuples(list);
    
    remove_tuples_with_length(list, 2);
    
    printf("\nAfter removing tuples with length 2:\n");
    print_tuples(list);
    
    free_tuple_list(list);
    return 0;
}