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

Node *create_alternate_list(Node *head) {
    if (head == NULL) {
        return NULL;
    }

    Node *new_head = create_node(head->data);
    Node *current = new_head;
    Node *original = head->next;

    int take = 0;
    while (original != NULL) {
        if (take) {
            current->next = create_node(original->data);
            current = current->next;
        }
        take = !take;
        original = original->next;
    }

    return new_head;
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    Node *head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);
    head->next->next->next->next = create_node(5);

    Node *alt_list = create_alternate_list(head);

    Node *curr = alt_list;
    while (curr != NULL) {
        printf("%d ", curr->data);
        curr = curr->next;
    }
    printf("\n");

    free_list(head);
    free_list(alt_list);

    return 0;
}