#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
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

Node *alternate_elements(Node *head) {
    Node *new_head = NULL;
    Node *new_tail = NULL;
    Node *current = head;
    int take = 1;

    while (current != NULL) {
        if (take) {
            Node *new_node = create_node(current->data);
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

int main(void) {
    Node *head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);
    head->next->next->next->next = create_node(5);

    Node *alt_list = alternate_elements(head);

    Node *current = alt_list;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");

    free_list(head);
    free_list(alt_list);

    return 0;
}