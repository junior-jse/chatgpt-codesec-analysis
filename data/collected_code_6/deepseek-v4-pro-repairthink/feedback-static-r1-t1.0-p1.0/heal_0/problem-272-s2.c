#include <stdio.h>
#include <stdlib.h>

typedef struct Tuple {
    int *elements;
    int size;
    struct Tuple *next;
} Tuple;

typedef struct {
    Tuple *head;
    Tuple *tail;
    int count;
} TupleList;

TupleList *create_list(void) {
    TupleList *list = (TupleList *)malloc(sizeof(TupleList));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

int append_tuple(TupleList *list, int *elements, int size) {
    if (list == NULL || elements == NULL || size <= 0) {
        return 0;
    }
    
    Tuple *new_tuple = (Tuple *)malloc(sizeof(Tuple));
    if (new_tuple == NULL) {
        return 0;
    }
    
    new_tuple->elements = (int *)malloc(sizeof(int) * size);
    if (new_tuple->elements == NULL) {
        free(new_tuple);
        return 0;
    }
    
    for (int i = 0; i < size; i++) {
        new_tuple->elements[i] = elements[i];
    }
    
    new_tuple->size = size;
    new_tuple->next = NULL;
    
    if (list->tail == NULL) {
        list->head = new_tuple;
        list->tail = new_tuple;
    } else {
        list->tail->next = new_tuple;
        list->tail = new_tuple;
    }
    
    list->count++;
    return 1;
}

int extract_rear_element(TupleList *list, int *result) {
    if (list == NULL || result == NULL || list->tail == NULL) {
        return 0;
    }
    
    Tuple *rear_tuple = list->tail;
    if (rear_tuple->size <= 0) {
        return 0;
    }
    
    *result = rear_tuple->elements[rear_tuple->size - 1];
    return 1;
}

void free_list(TupleList *list) {
    if (list == NULL) {
        return;
    }
    
    Tuple *current = list->head;
    while (current != NULL) {
        Tuple *next = current->next;
        free(current->elements);
        free(current);
        current = next;
    }
    
    free(list);
}

int main(void) {
    TupleList *list = create_list();
    if (list == NULL) {
        return 1;
    }
    
    int tuple1[] = {1, 2, 3};
    int tuple2[] = {4, 5, 6, 7};
    int tuple3[] = {8, 9};
    
    if (!append_tuple(list, tuple1, 3)) {
        free_list(list);
        return 1;
    }
    
    if (!append_tuple(list, tuple2, 4)) {
        free_list(list);
        return 1;
    }
    
    if (!append_tuple(list, tuple3, 2)) {
        free_list(list);
        return 1;
    }
    
    int rear_element;
    if (extract_rear_element(list, &rear_element)) {
        printf("Rear element from last tuple: %d\n", rear_element);
    } else {
        printf("Failed to extract rear element\n");
        free_list(list);
        return 1;
    }
    
    free_list(list);
    return 0;
}