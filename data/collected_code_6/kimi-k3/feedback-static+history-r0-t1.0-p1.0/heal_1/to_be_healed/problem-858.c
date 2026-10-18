#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int* data;
    size_t size;
    struct ListNode* next;
} ListNode;

typedef struct {
    ListNode* head;
    size_t count;
} ListOfLists;

int init_list_of_lists(ListOfLists* list) {
    if (list == NULL) {
        return -1;
    }
    list->head = NULL;
    list->count = 0;
    return 0;
}

int add_list(ListOfLists* list, const int* data, size_t size) {
    if (list == NULL) {
        return -1;
    }
    
    ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
    if (new_node == NULL) {
        return -1;
    }
    
    if (data != NULL && size > 0) {
        new_node->data = (int*)malloc(size * sizeof(int));
        if (new_node->data == NULL) {
            free(new_node);
            return -1;
        }
        for (size_t i = 0; i < size; i++) {
            new_node->data[i] = data[i];
        }
        new_node->size = size;
    } else {
        new_node->data = NULL;
        new_node->size = 0;
    }
    
    new_node->next = list->head;
    list->head = new_node;
    list->count++;
    return 0;
}

size_t count_lists(const ListOfLists* list) {
    if (list == NULL) {
        return 0;
    }
    return list->count;
}

size_t square_count(size_t count) {
    return count * count;
}

void free_list_of_lists(ListOfLists* list) {
    if (list == NULL) {
        return;
    }
    
    ListNode* current = list->head;
    while (current != NULL) {
        ListNode* temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }
    list->head = NULL;
    list->count = 0;
}

int main(void) {
    ListOfLists my_lists;
    
    if (init_list_of_lists(&my_lists) != 0) {
        fprintf(stderr, "Failed to initialize list\n");
        return EXIT_FAILURE;
    }
    
    /* Possible weaknesses found:
     *  Variable 'arr1' can be declared as const array [constVariable]
     */
    int arr1[] = {1, 2, 3};
    /* Possible weaknesses found:
     *  Variable 'arr2' can be declared as const array [constVariable]
     */
    int arr2[] = {4, 5};
    /* Possible weaknesses found:
     *  Variable 'arr3' can be declared as const array [constVariable]
     */
    int arr3[] = {6, 7, 8, 9};
    
    if (add_list(&my_lists, arr1, 3) != 0) {
        fprintf(stderr, "Failed to add list\n");
        free_list_of_lists(&my_lists);
        return EXIT_FAILURE;
    }
    
    if (add_list(&my_lists, arr2, 2) != 0) {
        fprintf(stderr, "Failed to add list\n");
        free_list_of_lists(&my_lists);
        return EXIT_FAILURE;
    }
    
    if (add_list(&my_lists, arr3, 4) != 0) {
        fprintf(stderr, "Failed to add list\n");
        free_list_of_lists(&my_lists);
        return EXIT_FAILURE;
    }
    
    size_t count = count_lists(&my_lists);
    size_t squared = square_count(count);
    
    printf("Number of lists: %zu\n", count);
    printf("Squared count: %zu\n", squared);
    
    free_list_of_lists(&my_lists);
    
    return EXIT_SUCCESS;
}