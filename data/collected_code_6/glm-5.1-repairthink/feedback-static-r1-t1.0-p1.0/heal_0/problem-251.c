#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Node *create_node(int data) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

struct Node *insert_before_each(struct Node *head, int val) {
    struct Node new_head;
    new_head.next = head;
    struct Node *current = &new_head;

    while (current->next != NULL) {
        struct Node *new_node = create_node(val);
        new_node->next = current->next;
        current->next = new_node;
        current = new_node->next;
    }

    return new_head.next;
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

    print_list(head);

    head = insert_before_each(head, 9);

    print_list(head);

    free_list(head);

    return 0;
}