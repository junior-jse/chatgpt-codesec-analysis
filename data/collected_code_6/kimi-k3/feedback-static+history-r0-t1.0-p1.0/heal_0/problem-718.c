#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

Node* create_alternate_list(Node* head) {
    if (head == NULL) {
        return NULL;
    }
    
    Node* new_head = NULL;
    Node* new_tail = NULL;
    Node* current = head;
    int take = 1;
    
    while (current != NULL) {
        if (take) {
            Node* new_node = create_node(current->data);
            if (new_head == NULL) {
                new_head = new_node;
                new_tail = new_node;
            } else {
                new_tail->next = new_node;
                new_tail = new_node;
            }
        }
        take = !take;
        current = current->next;
    }
    
    return new_head;
}

void print_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

int main(void) {
    Node* head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);
    head->next->next->next->next = create_node(5);
    head->next->next->next->next->next = create_node(6);
    head->next->next->next->next->next->next = create_node(7);
    
    printf("Original list: ");
    print_list(head);
    
    Node* alternate_list = create_alternate_list(head);
    
    printf("Alternate elements list: ");
    print_list(alternate_list);
    
    free_list(head);
    free_list(alternate_list);
    
    return EXIT_SUCCESS;
}