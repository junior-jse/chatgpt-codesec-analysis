#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntList;

typedef struct {
    IntList *lists;
    size_t size;
    size_t capacity;
} ListOfLists;

int init_list(IntList *list, size_t capacity) {
    if (list == NULL) {
        return -1;
    }
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
    
    if (capacity > 0) {
        list->data = malloc(capacity * sizeof(int));
        if (list->data == NULL) {
            return -1;
        }
        list->capacity = capacity;
    }
    return 0;
}

int init_list_of_lists(ListOfLists *lol, size_t capacity) {
    if (lol == NULL) {
        return -1;
    }
    lol->lists = NULL;
    lol->size = 0;
    lol->capacity = 0;
    
    if (capacity > 0) {
        lol->lists = malloc(capacity * sizeof(IntList));
        if (lol->lists == NULL) {
            return -1;
        }
        lol->capacity = capacity;
    }
    return 0;
}

int add_list(ListOfLists *lol, const IntList *list) {
    if (lol == NULL || list == NULL) {
        return -1;
    }
    
    if (lol->size >= lol->capacity) {
        size_t new_capacity = (lol->capacity == 0) ? 4 : lol->capacity * 2;
        IntList *new_lists = realloc(lol->lists, new_capacity * sizeof(IntList));
        if (new_lists == NULL) {
            return -1;
        }
        lol->lists = new_lists;
        lol->capacity = new_capacity;
    }
    
    lol->lists[lol->size].data = list->data;
    lol->lists[lol->size].size = list->size;
    lol->lists[lol->size].capacity = list->capacity;
    lol->size++;
    
    return 0;
}

int remove_empty_lists(ListOfLists *lol) {
    if (lol == NULL) {
        return -1;
    }
    
    size_t write_idx = 0;
    for (size_t read_idx = 0; read_idx < lol->size; read_idx++) {
        if (lol->lists[read_idx].size > 0) {
            if (write_idx != read_idx) {
                lol->lists[write_idx] = lol->lists[read_idx];
            }
            write_idx++;
        } else {
            free(lol->lists[read_idx].data);
            lol->lists[read_idx].data = NULL;
            lol->lists[read_idx].size = 0;
            lol->lists[read_idx].capacity = 0;
        }
    }
    
    for (size_t i = write_idx; i < lol->size; i++) {
        lol->lists[i].data = NULL;
        lol->lists[i].size = 0;
        lol->lists[i].capacity = 0;
    }
    
    lol->size = write_idx;
    return 0;
}

void free_list(IntList *list) {
    if (list != NULL) {
        free(list->data);
        list->data = NULL;
        list->size = 0;
        list->capacity = 0;
    }
}

void free_list_of_lists(ListOfLists *lol) {
    if (lol != NULL) {
        for (size_t i = 0; i < lol->size; i++) {
            free_list(&lol->lists[i]);
        }
        free(lol->lists);
        lol->lists = NULL;
        lol->size = 0;
        lol->capacity = 0;
    }
}

void print_list_of_lists(const ListOfLists *lol) {
    if (lol == NULL) {
        return;
    }
    
    printf("[");
    for (size_t i = 0; i < lol->size; i++) {
        printf("[");
        for (size_t j = 0; j < lol->lists[i].size; j++) {
            printf("%d", lol->lists[i].data[j]);
            if (j < lol->lists[i].size - 1) {
                printf(", ");
            }
        }
        printf("]");
        if (i < lol->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(void) {
    ListOfLists lol;
    IntList list1, list2, list3, list4;
    
    if (init_list_of_lists(&lol, 4) != 0) {
        fprintf(stderr, "Failed to initialize list of lists\n");
        return EXIT_FAILURE;
    }
    
    if (init_list(&list1, 3) != 0) {
        fprintf(stderr, "Failed to initialize list1\n");
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }
    list1.data[0] = 1;
    list1.data[1] = 2;
    list1.data[2] = 3;
    list1.size = 3;
    
    if (init_list(&list2, 0) != 0) {
        fprintf(stderr, "Failed to initialize list2\n");
        free_list(&list1);
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }
    
    if (init_list(&list3, 2) != 0) {
        fprintf(stderr, "Failed to initialize list3\n");
        free_list(&list1);
        free_list(&list2);
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }
    list3.data[0] = 4;
    list3.data[1] = 5;
    list3.size = 2;
    
    if (init_list(&list4, 0) != 0) {
        fprintf(stderr, "Failed to initialize list4\n");
        free_list(&list1);
        free_list(&list2);
        free_list(&list3);
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }
    
    if (add_list(&lol, &list1) != 0 ||
        add_list(&lol, &list2) != 0 ||
        add_list(&lol, &list3) != 0 ||
        add_list(&lol, &list4) != 0) {
        fprintf(stderr, "Failed to add lists\n");
        free_list(&list1);
        free_list(&list2);
        free_list(&list3);
        free_list(&list4);
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }
    
    printf("Original: ");
    print_list_of_lists(&lol);
    
    if (remove_empty_lists(&lol) != 0) {
        fprintf(stderr, "Failed to remove empty lists\n");
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }
    
    printf("After removal: ");
    print_list_of_lists(&lol);
    
    free_list_of_lists(&lol);
    
    return EXIT_SUCCESS;
}