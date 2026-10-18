#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int *data;
    int size;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} ListOfLists;

Node *create_node(const int *data, int size) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) return NULL;
    new_node->data = (int *)malloc(sizeof(int) * size);
    if (!new_node->data) {
        free(new_node);
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        new_node->data[i] = data[i];
    }
    new_node->size = size;
    new_node->next = NULL;
    return new_node;
}

void append_node(ListOfLists *list, const int *data, int size) {
    Node *new_node = create_node(data, size);
    if (!new_node) return;
    if (!list->head) {
        list->head = new_node;
    } else {
        Node *current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void sort_by_index(ListOfLists *list, int index) {
    if (!list || !list->head || !list->head->next) return;
    
    int swapped;
    Node *ptr1;
    Node *lptr = NULL;
    
    do {
        swapped = 0;
        ptr1 = list->head;
        
        while (ptr1->next != lptr) {
            if (index < ptr1->size && index < ptr1->next->size) {
                if (ptr1->data[index] > ptr1->next->data[index]) {
                    int *temp_data = ptr1->data;
                    int temp_size = ptr1->size;
                    
                    ptr1->data = ptr1->next->data;
                    ptr1->size = ptr1->next->size;
                    
                    ptr1->next->data = temp_data;
                    ptr1->next->size = temp_size;
                    
                    swapped = 1;
                }
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void free_list(ListOfLists *list) {
    Node *current = list->head;
    while (current) {
        Node *temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }
    list->head = NULL;
}

void print_list(ListOfLists *list) {
    Node *current = list->head;
    while (current) {
        printf("[");
        for (int i = 0; i < current->size; i++) {
            printf("%d", current->data[i]);
            if (i < current->size - 1) printf(", ");
        }
        printf("] ");
        current = current->next;
    }
    printf("\n");
}

int main() {
    ListOfLists list = {NULL};
    
    /* Possible weaknesses found:
     *  Variable 'arr1' can be declared as const array [constVariable]
     */
    int arr1[] = {5, 8, 1};
    /* Possible weaknesses found:
     *  Variable 'arr2' can be declared as const array [constVariable]
     */
    int arr2[] = {2, 7, 9};
    /* Possible weaknesses found:
     *  Variable 'arr3' can be declared as const array [constVariable]
     */
    int arr3[] = {4, 6, 3};
    /* Possible weaknesses found:
     *  Variable 'arr4' can be declared as const array [constVariable]
     */
    int arr4[] = {1, 2, 7};
    
    append_node(&list, arr1, 3);
    append_node(&list, arr2, 3);
    append_node(&list, arr3, 3);
    append_node(&list, arr4, 3);
    
    printf("Before sorting: ");
    print_list(&list);
    
    sort_by_index(&list, 0);
    
    printf("After sorting by index 0: ");
    print_list(&list);
    
    sort_by_index(&list, 2);
    
    printf("After sorting by index 2: ");
    print_list(&list);
    
    free_list(&list);
    
    return 0;
}