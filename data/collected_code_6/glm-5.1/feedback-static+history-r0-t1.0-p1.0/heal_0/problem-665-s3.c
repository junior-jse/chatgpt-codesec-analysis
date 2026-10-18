#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Node *shift_first_to_end(struct Node *head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    struct Node *first = head;
    struct Node *last = head;

    while (last->next != NULL) {
        last = last->next;
    }

    head = first->next;
    first->next = NULL;
    last->next = first;

    return head;
}

struct Node *create_node(int data) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void print_list(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void free_list(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *next = current->next;
        free(current);
        current = next;
    }
}

int main(void) {
    struct Node *head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);

    print_list(head);

    head = shift_first_to_end(head);

    print_list(head);

    free_list(head);

    return 0;
}