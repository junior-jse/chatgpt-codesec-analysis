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

Node *create_node(int *data, int size) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->size = size;
    node->next = NULL;
    return node;
}

void append_node(ListOfLists *list, Node *node) {
    if (list->head == NULL) {
        list->head = node;
    } else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }
}

void free_list(ListOfLists *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    list->head = NULL;
}

int sublist_sum(Node *node) {
    int sum = 0;
    for (int i = 0; i < node->size; i++) {
        sum += node->data[i];
    }
    return sum;
}

void remove_sublists_outside_range(ListOfLists *list, int min_val, int max_val) {
    Node *current = list->head;
    Node *prev = NULL;
    
    while (current != NULL) {
        int sum = sublist_sum(current);
        if (sum < min_val || sum > max_val) {
            Node *to_delete = current;
            if (prev == NULL) {
                list->head = current->next;
            } else {
                prev->next = current->next;
            }
            current = current->next;
            free(to_delete->data);
            free(to_delete);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void print_list(ListOfLists *list) {
    Node *current = list->head;
    while (current != NULL) {
        printf("[");
        for (int i = 0; i < current->size; i++) {
            printf("%d", current->data[i]);
            if (i < current->size - 1) {
                printf(", ");
            }
        }
        printf("] ");
        current = current->next;
    }
    printf("\n");
}

int *create_array(const int *values, int size) {
    int *arr = (int *)malloc(sizeof(int) * size);
    if (arr == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        arr[i] = values[i];
    }
    return arr;
}

int main() {
    ListOfLists list;
    list.head = NULL;
    
    /* Possible weaknesses found:
     *  Variable 'values1' can be declared as const array [constVariable]
     */
    int values1[] = {1, 2, 3};
    /* Possible weaknesses found:
     *  Variable 'values2' can be declared as const array [constVariable]
     */
    int values2[] = {10, 20};
    /* Possible weaknesses found:
     *  Variable 'values3' can be declared as const array [constVariable]
     */
    int values3[] = {5, 5, 5, 5};
    /* Possible weaknesses found:
     *  Variable 'values4' can be declared as const array [constVariable]
     */
    int values4[] = {100};
    
    int *arr1 = create_array(values1, 3);
    int *arr2 = create_array(values2, 2);
    int *arr3 = create_array(values3, 4);
    int *arr4 = create_array(values4, 1);
    
    if (!arr1 || !arr2 || !arr3 || !arr4) {
        free(arr1);
        free(arr2);
        free(arr3);
        free(arr4);
        return 1;
    }
    
    Node *n1 = create_node(arr1, 3);
    Node *n2 = create_node(arr2, 2);
    Node *n3 = create_node(arr3, 4);
    Node *n4 = create_node(arr4, 1);
    
    if (!n1 || !n2 || !n3 || !n4) {
        free(arr1);
        free(arr2);
        free(arr3);
        free(arr4);
        free(n1);
        free(n2);
        free(n3);
        free(n4);
        return 1;
    }
    
    append_node(&list, n1);
    append_node(&list, n2);
    append_node(&list, n3);
    append_node(&list, n4);
    
    printf("Original list: ");
    print_list(&list);
    
    remove_sublists_outside_range(&list, 10, 50);
    
    printf("List after removing sublists outside range [10, 50]: ");
    print_list(&list);
    
    free_list(&list);
    
    return 0;
}