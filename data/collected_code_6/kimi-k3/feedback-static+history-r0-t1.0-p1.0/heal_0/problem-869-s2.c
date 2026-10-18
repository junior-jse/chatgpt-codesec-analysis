#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
} SubList;

typedef struct {
    SubList *lists;
    size_t count;
    size_t capacity;
} ListOfLists;

int init_list_of_lists(ListOfLists *list, size_t initial_capacity) {
    if (list == NULL || initial_capacity == 0) {
        return -1;
    }
    list->lists = malloc(initial_capacity * sizeof(SubList));
    if (list->lists == NULL) {
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
        SubList *new_lists = realloc(list->lists, new_capacity * sizeof(SubList));
        if (new_lists == NULL) {
            return -1;
        }
        list->lists = new_lists;
        list->capacity = new_capacity;
    }
    
    list->lists[list->count].data = NULL;
    if (size > 0) {
        list->lists[list->count].data = malloc(size * sizeof(int));
        if (list->lists[list->count].data == NULL) {
            return -1;
        }
        memcpy(list->lists[list->count].data, data, size * sizeof(int));
    }
    list->lists[list->count].size = size;
    list->count++;
    return 0;
}

int get_sublist_sum(const SubList *sublist, long *sum) {
    if (sublist == NULL || sum == NULL) {
        return -1;
    }
    *sum = 0;
    for (size_t i = 0; i < sublist->size; i++) {
        *sum += sublist->data[i];
    }
    return 0;
}

int remove_sublists_outside_range(ListOfLists *list, long min_val, long max_val) {
    if (list == NULL || min_val > max_val) {
        return -1;
    }
    
    size_t write_idx = 0;
    for (size_t read_idx = 0; read_idx < list->count; read_idx++) {
        long sum;
        if (get_sublist_sum(&list->lists[read_idx], &sum) != 0) {
            return -1;
        }
        
        if (sum >= min_val && sum <= max_val) {
            if (write_idx != read_idx) {
                list->lists[write_idx] = list->lists[read_idx];
            }
            write_idx++;
        } else {
            free(list->lists[read_idx].data);
            list->lists[read_idx].data = NULL;
        }
    }
    
    for (size_t i = write_idx; i < list->count; i++) {
        list->lists[i].data = NULL;
        list->lists[i].size = 0;
    }
    
    list->count = write_idx;
    return 0;
}

void free_list_of_lists(ListOfLists *list) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < list->count; i++) {
        free(list->lists[i].data);
        list->lists[i].data = NULL;
    }
    free(list->lists);
    list->lists = NULL;
    list->count = 0;
    list->capacity = 0;
}

void print_list_of_lists(const ListOfLists *list) {
    if (list == NULL) {
        return;
    }
    printf("[");
    for (size_t i = 0; i < list->count; i++) {
        printf("[");
        for (size_t j = 0; j < list->lists[i].size; j++) {
            printf("%d", list->lists[i].data[j]);
            if (j < list->lists[i].size - 1) {
                printf(", ");
            }
        }
        printf("]");
        if (i < list->count - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(void) {
    ListOfLists list;
    if (init_list_of_lists(&list, 4) != 0) {
        fprintf(stderr, "Failed to initialize list\n");
        return EXIT_FAILURE;
    }
    
    int arr1[] = {1, 2, 3};
    int arr2[] = {10, 20};
    int arr3[] = {5};
    int arr4[] = {100, 200, 300};
    int arr5[] = {7, 8};
    
    if (add_sublist(&list, arr1, 3) != 0 ||
        add_sublist(&list, arr2, 2) != 0 ||
        add_sublist(&list, arr3, 1) != 0 ||
        add_sublist(&list, arr4, 3) != 0 ||
        add_sublist(&list, arr5, 2) != 0) {
        fprintf(stderr, "Failed to add sublist\n");
        free_list_of_lists(&list);
        return EXIT_FAILURE;
    }
    
    printf("Original: ");
    print_list_of_lists(&list);
    
    long min_range = 10;
    long max_range = 50;
    
    if (remove_sublists_outside_range(&list, min_range, max_range) != 0) {
        fprintf(stderr, "Failed to remove sublists\n");
        free_list_of_lists(&list);
        return EXIT_FAILURE;
    }
    
    printf("After removing sums outside [%ld, %ld]: ", min_range, max_range);
    print_list_of_lists(&list);
    
    free_list_of_lists(&list);
    return EXIT_SUCCESS;
}