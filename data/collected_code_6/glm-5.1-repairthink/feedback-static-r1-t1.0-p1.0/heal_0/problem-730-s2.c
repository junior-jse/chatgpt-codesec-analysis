#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

void remove_consecutive_duplicates(Node *head) {
    if (head == NULL) {
        return;
    }
    Node *current = head;
    while (current->next != NULL) {
        if (current->data == current->next->data) {
            Node *temp = current->next;
            current->next = temp->next;
            free(temp);
        } else {
            current = current->next;
        }
    }
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    Node *head = create_node(1);
    head->next = create_node(1);
    head->next->next = create_node(2);
    head->next->next->next = create_node(3);
    head->next->next->next->next = create_node(3);
    head->next->next->next->next->next = create_node(3);
    head->next->next->next->next->next->next = create_node(4);

    print_list(head);
    remove_consecutive_duplicates(head);
    print_list(head);

    free_list(head);
    return 0;
}