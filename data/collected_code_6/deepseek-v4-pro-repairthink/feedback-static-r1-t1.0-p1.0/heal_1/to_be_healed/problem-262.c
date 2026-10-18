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

Node* split_list(Node* head, int length) {
    if (head == NULL || length <= 0) {
        return head;
    }
    
    Node* current = head;
    /* Possible weaknesses found:
     *  Variable 'first_part' is assigned a value that is never used. [unreadVariable]
     *  Variable 'first_part' can be declared as pointer to const [constVariablePointer]
     */
    Node* first_part = NULL;
    Node* second_part = NULL;
    
    if (length == 1) {
        second_part = head->next;
        head->next = NULL;
        return second_part;
    }
    
    for (int i = 1; i < length; i++) {
        if (current->next == NULL) {
            return NULL;
        }
        current = current->next;
    }
    
    second_part = current->next;
    current->next = NULL;
    
    return second_part;
}

void print_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
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

int main() {
    Node* head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);
    head->next->next->next->next = create_node(5);
    
    printf("Original list: ");
    print_list(head);
    
    Node* second_part = split_list(head, 3);
    
    printf("First part: ");
    print_list(head);
    
    printf("Second part: ");
    print_list(second_part);
    
    free_list(head);
    free_list(second_part);
    
    return 0;
}