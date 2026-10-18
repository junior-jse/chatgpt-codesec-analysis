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

Node *remove_odd_numbers(Node *head) {
    Node *current = head;
    Node *prev = NULL;

    while (current != NULL) {
        if (current->data % 2 != 0) {
            Node *temp = current;
            if (prev == NULL) {
                head = current->next;
                current = head;
            } else {
                prev->next = current->next;
                current = current->next;
            }
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
    return head;
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int main(void) {
    Node *list = NULL;

    append_node(&list, 1);
    append_node(&list, 2);
    append_node(&list, 3);
    append_node(&list, 4);
    append_node(&list, 5);
    append_node(&list, 6);

    printf("Original list: ");
    print_list(list);

    list = remove_odd_numbers(list);

    printf("List after removing odd numbers: ");
    print_list(list);

    free_list(list);

    return 0;
}