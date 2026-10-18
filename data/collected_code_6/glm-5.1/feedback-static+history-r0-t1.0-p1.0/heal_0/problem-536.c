#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node* create_node(int data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void append_node(Node **head, int data) {
    Node *new_node = create_node(data);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

Node* select_nth(Node *head, size_t n) {
    if (head == NULL) {
        return NULL;
    }
    Node *current = head;
    size_t count = 0;
    while (current != NULL && count < n) {
        current = current->next;
        count++;
    }
    return current;
}

void free_list(Node *head) {
    Node *current = head;
    Node *next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}

int main(void) {
    Node *head = NULL;
    append_node(&head, 10);
    append_node(&head, 20);
    append_node(&head, 30);
    append_node(&head, 40);

    size_t n = 2;
    Node *result = select_nth(head, n);
    if (result != NULL) {
        printf("Item at index %zu is %d\n", n, result->data);
    } else {
        printf("Index %zu out of bounds\n", n);
    }

    n = 5;
    result = select_nth(head, n);
    if (result != NULL) {
        printf("Item at index %zu is %d\n", n, result->data);
    } else {
        printf("Index %zu out of bounds\n", n);
    }

    free_list(head);

    return 0;
}