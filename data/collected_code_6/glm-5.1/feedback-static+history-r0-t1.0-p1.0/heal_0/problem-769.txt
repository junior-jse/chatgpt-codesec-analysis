#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *items;
    size_t size;
    size_t capacity;
} List;

List list_create(size_t initial_capacity) {
    List list;
    list.capacity = initial_capacity > 0 ? initial_capacity : 1;
    list.items = (int *)malloc(list.capacity * sizeof(int));
    if (list.items == NULL) {
        exit(EXIT_FAILURE);
    }
    list.size = 0;
    return list;
}

void list_append(List *list, int item) {
    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity * 2;
        int *new_items = (int *)realloc(list->items, new_capacity * sizeof(int));
        if (new_items == NULL) {
            exit(EXIT_FAILURE);
        }
        list->items = new_items;
        list->capacity = new_capacity;
    }
    list->items[list->size++] = item;
}

void list_free(List *list) {
    if (list->items != NULL) {
        free(list->items);
        list->items = NULL;
    }
    list->size = 0;
    list->capacity = 0;
}

List get_difference(const List *list1, const List *list2) {
    List diff = list_create(list1->size > 0 ? list1->size : 1);
    
    for (size_t i = 0; i < list1->size; i++) {
        int found = 0;
        for (size_t j = 0; j < list2->size; j++) {
            if (list1->items[i] == list2->items[j]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            list_append(&diff, list1->items[i]);
        }
    }
    
    return diff;
}

int main(void) {
    List list1 = list_create(4);
    List list2 = list_create(4);
    
    list_append(&list1, 1);
    list_append(&list1, 2);
    list_append(&list1, 3);
    list_append(&list1, 4);
    list_append(&list1, 5);
    
    list_append(&list2, 4);
    list_append(&list2, 5);
    list_append(&list2, 6);
    list_append(&list2, 7);
    
    List diff = get_difference(&list1, &list2);
    
    for (size_t i = 0; i < diff.size; i++) {
        printf("%d ", diff.items[i]);
    }
    printf("\n");
    
    list_free(&list1);
    list_free(&list2);
    list_free(&diff);
    
    return 0;
}