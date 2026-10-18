#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *items;
    size_t size;
    size_t capacity;
} IntList;

IntList create_list(size_t capacity) {
    IntList list;
    list.items = (int *)malloc(capacity * sizeof(int));
    if (list.items == NULL) {
        exit(EXIT_FAILURE);
    }
    list.size = 0;
    list.capacity = capacity;
    return list;
}

void append_list(IntList *list, int value) {
    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 1 : list->capacity * 2;
        int *new_items = (int *)realloc(list->items, new_capacity * sizeof(int));
        if (new_items == NULL) {
            free(list->items);
            exit(EXIT_FAILURE);
        }
        list->items = new_items;
        list->capacity = new_capacity;
    }
    list->items[list->size++] = value;
}

void free_list(IntList *list) {
    free(list->items);
    list->items = NULL;
    list->size = 0;
    list->capacity = 0;
}

IntList get_difference(IntList *list1, IntList *list2) {
    IntList result = create_list(list1->size > 0 ? list1->size : 1);
    
    for (size_t i = 0; i < list1->size; i++) {
        int found = 0;
        for (size_t j = 0; j < list2->size; j++) {
            if (list1->items[i] == list2->items[j]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            append_list(&result, list1->items[i]);
        }
    }
    
    return result;
}

int main(void) {
    IntList list1 = create_list(4);
    IntList list2 = create_list(4);
    
    append_list(&list1, 1);
    append_list(&list1, 2);
    append_list(&list1, 3);
    append_list(&list1, 4);
    
    append_list(&list2, 3);
    append_list(&list2, 4);
    append_list(&list2, 5);
    append_list(&list2, 6);
    
    IntList diff = get_difference(&list1, &list2);
    
    for (size_t i = 0; i < diff.size; i++) {
        printf("%d ", diff.items[i]);
    }
    printf("\n");
    
    free_list(&list1);
    free_list(&list2);
    free_list(&diff);
    
    return 0;
}