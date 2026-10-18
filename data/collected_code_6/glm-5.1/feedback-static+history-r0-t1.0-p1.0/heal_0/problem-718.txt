#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

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

Node* take_alternate(Node* head) {
    if (head == NULL) {
        return NULL;
    }
    
    Node* new_head = create_node(head->data);
    Node* new_current = new_head;
    Node* current = head->next;
    int take = 0;
    
    while (current != NULL) {
        if (take) {
            new_current->next = create_node(current->data);
            if (new_current->next == NULL) {
                free_list(new_head);
                exit(EXIT_FAILURE);
            }
            new_current = new_current->next;
        }
        take = !take;
        current = current->next;
    }
    
    return new_head;
}

int main(void) {
    Node* head = create_node(1);
    Node* current = head;
    for (int i = 2; i <= 6; i++) {
        current->next = create_node(i);
        if (current->next == NULL) {
            free_list(head);
            return EXIT_FAILURE;
        }
        current = current->next;
    }
    
    Node* alternate_list = take_alternate(head);
    
    current = alternate_list;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
    
    free_list(head);
    free_list(alternate_list);
    
    return EXIT_SUCCESS;
}