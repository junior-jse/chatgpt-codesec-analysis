#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int *data;
    int size;
    struct Node *next;
} Node;

Node *create_node(int *arr, int size) {
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
    
    current = head;
    while (current) {
        if (col < current->size) {
            int new_size = current->size - 1;
            int *new_data = (int *)malloc(new_size * sizeof(int));
            if (!new_data) return head;
            
            for (int i = 0, j = 0; i < current->size; i++) {
                if (i != col) {
                    new_data[j++] = current->data[i];
                }
            }
            
            free(current->data);
            current->data = new_data;
            current->size = new_size;
        }
        current = current->next;
    }
    
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
    int arr1[] = {1, 2, 3, 4};
    int arr2[] = {5, 6, 7};
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