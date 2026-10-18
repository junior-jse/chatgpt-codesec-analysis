#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void insert_before_each(Node **head, int value) {
    if (head == NULL || *head == NULL) {
        return;
    }

    Node *current = *head;
    Node *new_head = NULL;
    Node *new_tail = NULL;

    while (current != NULL) {
        Node *new_node = create_node(value);
        if (new_head == NULL) {
            new_head = new_node;
            new_tail = new_node;
        } else {
            new_tail->next = new_node;
            new_tail = new_node;
        }

        Node *copy = create_node(current->data);
        new_tail->next = copy;
        new_tail = copy;

        current = current->next;
    }

    Node *temp = *head;
    while (temp != NULL) {
        Node *next = temp->next;
        free(temp);
        temp = next;
    }

    *head = new_head;
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

int main(void) {
    Node *head = NULL;
    Node *tail = NULL;

    int values[] = {1, 2, 3};

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        Node *new_node = create_node(values[i]);
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    printf("Original list: ");
    print_list(head);

    insert_before_each(&head, 0);

    printf("Modified list: ");
    print_list(head);

    free_list(head);

    return 0;
}