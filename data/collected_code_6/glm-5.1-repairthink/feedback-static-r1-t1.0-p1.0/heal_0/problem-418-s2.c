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

void append_node(Node **head, int data) {
    if (head == NULL) {
        return;
    }
    if (*head == NULL) {
        *head = create_node(data);
        return;
    }
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = create_node(data);
}

Node *find_max_length_sublist(Node *head) {
    if (head == NULL) {
        return NULL;
    }

    Node *max_start = NULL;
    size_t max_len = 0;
    
    Node *current_start = head;
    size_t current_len = 1;

    Node *prev = head;
    Node *current = head->next;

    while (current != NULL) {
        if (current->data >= prev->data) {
            current_len++;
        } else {
            if (current_len > max_len) {
                max_len = current_len;
                max_start = current_start;
            }
            current_start = current;
            current_len = 1;
        }
        prev = current;
        current = current->next;
    }

    if (current_len > max_len) {
        max_len = current_len;
        max_start = current_start;
    }

    return max_start;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

void print_sublist(Node *start) {
    if (start == NULL) {
        return;
    }

    Node *current = start;
    Node *prev = current;
    printf("%d", current->data);
    current = current->next;

    while (current != NULL && current->data >= prev->data) {
        printf(" -> %d", current->data);
        prev = current;
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    Node *head = NULL;

    append_node(&head, 1);
    append_node(&head, 2);
    append_node(&head, 3);
    append_node(&head, 1);
    append_node(&head, 2);
    append_node(&head, 3);
    append_node(&head, 4);
    append_node(&head, 1);
    append_node(&head, 2);

    Node *max_sublist = find_max_length_sublist(head);
    
    print_sublist(max_sublist);

    free_list(head);

    return 0;
}