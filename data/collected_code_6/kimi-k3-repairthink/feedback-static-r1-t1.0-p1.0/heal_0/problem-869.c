#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
} SubList;

typedef struct {
    SubList *items;
    size_t count;
    size_t capacity;
} ListOfLists;

int init_list(ListOfLists *list, size_t initial_capacity) {
    if (list == NULL || initial_capacity == 0) {
        return -1;
    }
    list->items = malloc(initial_capacity * sizeof(SubList));
    if (list->items == NULL) {
        return -1;
    }
    list->count = 0;
    list->capacity = initial_capacity;
    return 0;
}

int add_sublist(ListOfLists *list, const int *data, size_t size) {
    if (list == NULL || (data == NULL && size > 0)) {
        return -1;
    }
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        SubList *new_items = realloc(list->items, new_capacity * sizeof(SubList));
        if (new_items == NULL) {
            return -1;
        }
        list->items = new_items;
        list->capacity = new_capacity;
    }
    
    int *new_data = NULL;
    if (size > 0) {
        new_data = malloc(size * sizeof(int));
        if (new_data == NULL) {
            return -1;
        }
        memcpy(new_data, data, size * sizeof(int));
    }
    
    list->items[list->count].data = new_data;
    list->items[list->count].size = size;
    list->count++;
    return 0;
}

int remove_sublists_outside_range(ListOfLists *list, size_t min_idx, size_t max_idx) {
    if (list == NULL || min_idx > max_idx) {
        return -1;
    }
    
    if (min_idx >= list->count) {
        for (size_t i = 0; i < list->count; i++) {
            free(list->items[i].data);
        }
        list->count = 0;
        return 0;
    }
    
    size_t effective_max = max_idx;
    if (effective_max >= list->count) {
        effective_max = list->count - 1;
    }
    
    size_t write_idx = 0;
    for (size_t i = 0; i < list->count; i++) {
        if (i >= min_idx && i <= effective_max) {
            if (write_idx != i) {
                list->items[write_idx] = list->items[i];
            }
            write_idx++;
        } else {
            free(list->items[i].data);
        }
    }
    list->count = write_idx;
    return 0;
}

void free_list(ListOfLists *list) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < list->count; i++) {
        free(list->items[i].data);
    }
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

void print_list(const ListOfLists *list) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < list->count; i++) {
        printf("[");
        for (size_t j = 0; j < list->items[i].size; j++) {
            printf("%d", list->items[i].data[j]);
            if (j < list->items[i].size - 1) {
                printf(", ");
            }
        }
        printf("]");
        if (i < list->count - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

int main(void) {
    ListOfLists list;
    if (init_list(&list, 4) != 0) {
        fprintf(stderr, "Failed to initialize list\n");
        return EXIT_FAILURE;
    }
    
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5};
    int data3[] = {6, 7, 8, 9};
    int data4[] = {10};
    int data5[] = {11, 12};
    
    if (add_sublist(&list, data1, 3) != 0 ||
        add_sublist(&list, data2, 2) != 0 ||
        add_sublist(&list, data3, 4) != 0 ||
        add_sublist(&list, data4, 1) != 0 ||
        add_sublist(&list, data5, 2) != 0) {
        fprintf(stderr, "Failed to add sublist\n");
        free_list(&list);
        return EXIT_FAILURE;
    }
    
    printf("Original: ");
    print_list(&list);
    
    if (remove_sublists_outside_range(&list, 1, 3) != 0) {
        fprintf(stderr, "Failed to remove sublists\n");
        free_list(&list);
        return EXIT_FAILURE;
    }
    
    printf("After removal (indices 1-3): ");
    print_list(&list);
    
    free_list(&list);
    return EXIT_SUCCESS;
}