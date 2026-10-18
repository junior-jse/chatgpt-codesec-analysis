#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

int max_sublist_length(Node* head) {
    int max_len = 0;
    int current_len = 0;
    Node* current = head;

    while (current != NULL) {
        current_len++;
        if (current_len > max_len) {
            max_len = current_len;
        }
        current = current->next;
    }

    return max_len;
}

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_list(Node* head) {
    Node* current = head;
    Node* next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}

int main() {
    Node* head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);

    int length = max_sublist_length(head);
    printf("%d\n", length);

    free_list(head);
    return 0;
}