#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int *data;
    int size;
    struct Node *next;
} Node;

Node *create_node(const int *arr, int size) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) return NULL;
    node->data = (int *)malloc(size * sizeof(int));
    if (!node->data) {
        free(node);
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        node->data[i] = arr[i];
    }
    node->size = size;
    node->next = NULL;
    return node;
}

void free_list(Node *head) {
    while (head) {
        Node *temp = head;
        head = head->next;
        free(temp->data);
        free(temp);
    }
}

Node *remove_column(Node *head, int col) {
    if (!head || col < 0) return head;
    
    Node *current = head;
    int max_size = 0;
    while (current) {
        if (current->size > max_size) max_size = current->size;
        current = current->next;
    }
    
    if (col >= max_size) return head;
    
    int num_nodes = 0;
    current = head;
    while (current) {
        num_nodes++;
        current = current->next;
    }
    
    int **new_datas = (int **)malloc(num_nodes * sizeof(int *));
    if (!new_datas) return head;
    
    current = head;
    int idx = 0;
    int success = 1;
    while (current) {
        if (col < current->size) {
            int new_size = current->size - 1;
            int *new_data = (int *)malloc(new_size * sizeof(int));
            if (!new_data) {
                success = 0;
                break;
            }
            for (int i = 0, j = 0; i < current->size; i++) {
                if (i != col) {
                    new_data[j++] = current->data[i];
                }
            }
            new_datas[idx] = new_data;
        } else {
            new_datas[idx] = NULL;
        }
        idx++;
        current = current->next;
    }
    
    if (!success) {
        for (int i = 0; i < idx; i++) {
            free(new_datas[i]);
        }
        free(new_datas);
        return head;
    }
    
    current = head;
    idx = 0;
    while (current) {
        if (new_datas[idx] != NULL) {
            free(current->data);
            current->data = new_datas[idx];
            current->size = current->size - 1;
        }
        idx++;
        current = current->next;
    }
    
    free(new_datas);
    return head;
}

void print_list(Node *head) {
    Node *current = head;
    while (current) {
        for (int i = 0; i < current->size; i++) {
            printf("%d ", current->data[i]);
        }
        printf("\n");
        current = current->next;
    }
}

int main() {
    /* Possible weaknesses found:
     *  Variable 'arr1' can be declared as const array [constVariable]
     */
    int arr1[] = {1, 2, 3, 4};
    /* Possible weaknesses found:
     *  Variable 'arr2' can be declared as const array [constVariable]
     */
    int arr2[] = {5, 6, 7};
    /* Possible weaknesses found:
     *  Variable 'arr3' can be declared as const array [constVariable]
     */
    int arr3[] = {8, 9, 10, 11, 12};
    
    Node *head = create_node(arr1, 4);
    head->next = create_node(arr2, 3);
    head->next->next = create_node(arr3, 5);
    
    printf("Original list:\n");
    print_list(head);
    
    head = remove_column(head, 2);
    
    printf("\nAfter removing column 2:\n");
    print_list(head);
    
    free_list(head);
    
    return 0;
}