#include <stdio.h>
#include <stdlib.h>

typedef struct Tuple {
    int *data;
    int size;
    struct Tuple *next;
} Tuple;

typedef struct {
    Tuple *head;
} TupleList;

TupleList* create_tuple_list(void) {
    TupleList *list = (TupleList*)malloc(sizeof(TupleList));
    if (list == NULL) return NULL;
    list->head = NULL;
    return list;
}

int add_tuple(TupleList *list, const int *data, int size) {
    if (list == NULL) return 0;
    
    Tuple *new_tuple = (Tuple*)malloc(sizeof(Tuple));
    if (new_tuple == NULL) return 0;
    
    new_tuple->size = size;
    new_tuple->next = NULL;
    
    if (size > 0) {
        new_tuple->data = (int*)malloc(sizeof(int) * size);
        if (new_tuple->data == NULL) {
            free(new_tuple);
            return 0;
        }
        for (int i = 0; i < size; i++) {
            new_tuple->data[i] = data[i];
        }
    } else {
        new_tuple->data = NULL;
    }
    
    if (list->head == NULL) {
        list->head = new_tuple;
    } else {
        Tuple *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_tuple;
    }
    return 1;
}

void remove_empty_tuples(TupleList *list) {
    if (list == NULL || list->head == NULL) return;
    
    Tuple *current = list->head;
    Tuple *prev = NULL;
    
    while (current != NULL) {
        if (current->size == 0) {
            Tuple *to_delete = current;
            if (prev == NULL) {
                list->head = current->next;
                current = list->head;
            } else {
                prev->next = current->next;
                current = current->next;
            }
            free(to_delete->data);
            free(to_delete);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void free_tuple_list(TupleList *list) {
    if (list == NULL) return;
    
    Tuple *current = list->head;
    while (current != NULL) {
        Tuple *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}

void print_tuple_list(TupleList *list) {
    if (list == NULL || list->head == NULL) {
        printf("[]\n");
        return;
    }
    
    Tuple *current = list->head;
    printf("[");
    while (current != NULL) {
        if (current->size == 0) {
            printf("()");
        } else {
            printf("(");
            for (int i = 0; i < current->size; i++) {
                printf("%d", current->data[i]);
                if (i < current->size - 1) printf(", ");
            }
            printf(")");
        }
        if (current->next != NULL) printf(", ");
        current = current->next;
    }
    printf("]\n");
}

int main(void) {
    TupleList *list = create_tuple_list();
    
    /* Possible weaknesses found:
     *  Variable 'data1' can be declared as const array [constVariable]
     */
    int data1[] = {1, 2, 3};
    /* Possible weaknesses found:
     *  Variable 'data2' can be declared as const array [constVariable]
     */
    int data2[] = {4};
    /* Possible weaknesses found:
     *  Variable 'data3' can be declared as const array [constVariable]
     */
    int data3[] = {5, 6};
    
    add_tuple(list, data1, 3);
    add_tuple(list, NULL, 0);
    add_tuple(list, data2, 1);
    add_tuple(list, NULL, 0);
    add_tuple(list, data3, 2);
    
    printf("Original list: ");
    print_tuple_list(list);
    
    remove_empty_tuples(list);
    
    printf("After removing empty tuples: ");
    print_tuple_list(list);
    
    free_tuple_list(list);
    
    return 0;
}