#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node* create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node != NULL) {
        node->data = data;
        node->next = NULL;
    }
    return node;
}

void free_list(Node *head) {
    Node *current = head;
    Node *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int find_max_sublist_length(Node *head) {
    int max_length = 0;
    int current_length = 0;
    Node *current = head;
    
    while (current != NULL) {
        current_length++;
        if (current_length > max_length) {
            max_length = current_length;
        }
        current = current->next;
    }
    
    return max_length;
}

int main() {
    Node *head = create_node(1);
    if (head == NULL) {
        return 1;
    }
    
    head->next = create_node(2);
    if (head->next == NULL) {
        free_list(head);
        return 1;
    }
    
    head->next->next = create_node(3);
    if (head->next->next == NULL) {
        free_list(head);
        return 1;
    }
    
    int max_len = find_max_sublist_length(head);
    printf("%d\n", max_len);
    
    free_list(head);
    
    return 0;
}