#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node* create_node(int data) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

Node* replace_last_with_list(Node *head1, Node *head2) {
    if (head2 == NULL) {
        return head1;
    }
    
    if (head1 == NULL) {
        return head2;
    }
    
    if (head1->next == NULL) {
        free(head1);
        return head2;
    }
    
    Node *current = head1;
    while (current->next->next != NULL) {
        current = current->next;
    }
    
    free(current->next);
    current->next = head2;
    
    return head1;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    Node *list1 = create_node(1);
    list1->next = create_node(2);
    list1->next->next = create_node(3);
    
    Node *list2 = create_node(4);
    list2->next = create_node(5);
    
    list1 = replace_last_with_list(list1, list2);
    
    Node *current = list1;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
    
    free_list(list1);
    
    return 0;
}