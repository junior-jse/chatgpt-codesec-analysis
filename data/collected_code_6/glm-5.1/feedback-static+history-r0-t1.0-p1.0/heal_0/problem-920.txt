#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int val1;
    int val2;
    int is_none1;
    int is_none2;
} Tuple;

typedef struct {
    Tuple *items;
    size_t count;
    size_t capacity;
} TupleList;

TupleList tuple_list_create(size_t initial_capacity) {
    TupleList list;
    list.items = (Tuple *)malloc(initial_capacity * sizeof(Tuple));
    if (list.items == NULL) {
        list.count = 0;
        list.capacity = 0;
        return list;
    }
    list.count = 0;
    list.capacity = initial_capacity;
    return list;
}

int tuple_list_append(TupleList *list, Tuple t) {
    if (list->count == list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 4 : list->capacity * 2;
        Tuple *new_items = (Tuple *)realloc(list->items, new_capacity * sizeof(Tuple));
        if (new_items == NULL) {
            return -1;
        }
        list->items = new_items;
        list->capacity = new_capacity;
    }
    list->items[list->count++] = t;
    return 0;
}

void tuple_list_destroy(TupleList *list) {
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

int remove_all_none_tuples(TupleList *list) {
    if (list == NULL || list->items == NULL) {
        return -1;
    }
    
    size_t write_index = 0;
    for (size_t read_index = 0; read_index < list->count; ++read_index) {
        Tuple current = list->items[read_index];
        if (!(current.is_none1 && current.is_none2)) {
            list->items[write_index] = current;
            write_index++;
        }
    }
    list->count = write_index;
    
    if (list->count < list->capacity) {
        size_t new_capacity = list->count == 0 ? 1 : list->count;
        Tuple *new_items = (Tuple *)realloc(list->items, new_capacity * sizeof(Tuple));
        if (new_items != NULL || new_capacity == 0) {
            list->items = new_items;
            list->capacity = new_capacity;
        }
    }
    
    return 0;
}

int main(void) {
    TupleList list = tuple_list_create(4);
    if (list.items == NULL && list.capacity == 0) {
        return 1;
    }
    
    Tuple t1 = {1, 2, 0, 0};
    Tuple t2 = {0, 0, 1, 1};
    Tuple t3 = {3, 4, 0, 0};
    Tuple t4 = {0, 0, 1, 0};
    Tuple t5 = {0, 0, 1, 1};
    
    if (tuple_list_append(&list, t1) != 0 ||
        tuple_list_append(&list, t2) != 0 ||
        tuple_list_append(&list, t3) != 0 ||
        tuple_list_append(&list, t4) != 0 ||
        tuple_list_append(&list, t5) != 0) {
        tuple_list_destroy(&list);
        return 1;
    }
    
    if (remove_all_none_tuples(&list) != 0) {
        tuple_list_destroy(&list);
        return 1;
    }
    
    for (size_t i = 0; i < list.count; ++i) {
        printf("Tuple %zu: val1=%d, val2=%d, is_none1=%d, is_none2=%d\n",
               i, list.items[i].val1, list.items[i].val2, 
               list.items[i].is_none1, list.items[i].is_none2);
    }
    
    tuple_list_destroy(&list);
    return 0;
}