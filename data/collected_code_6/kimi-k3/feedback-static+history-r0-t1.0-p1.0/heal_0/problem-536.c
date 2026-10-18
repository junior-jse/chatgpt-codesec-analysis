#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void append_node(Node** head, int data) {
    Node* new_node = create_node(data);
    if (new_node == NULL) {
        return;
    }
    
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    
    Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

int select_nth(Node* head, int n, int* result) {
    if (head == NULL || n < 0 || result == NULL) {
        return -1;
    }
    
    Node* current = head;
    int index = 0;
    
    while (current != NULL) {
        if (index == n) {
            *result = current->data;
            return 0;
        }
        current = current->next;
        index++;
    }
    
    return -1;
}

void free_list(Node* head) {
    Node* current = head;
    Node* next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main(void) {
    Node* head = NULL;
    int result;
    int n;
    
    append_node(&head, 10);
    append_node(&head, 20);
    append_node(&head, 30);
    append_node(&head, 40);
    append_node(&head, 50);
    
    n = 2;
    if (select_nth(head, n, &result) == 0) {
        printf("Item at index %d: %d\n", n, result);
    } else {
        printf("Index %d is out of bounds\n", n);
    }
    
    n = 10;
    if (select_nth(head, n, &result) == 0) {
        printf("Item at index %d: %d\n", n, result);
    } else {
        printf("Index %d is out of bounds\n", n);
    }
    
    free_list(head);
    
    return 0;
}