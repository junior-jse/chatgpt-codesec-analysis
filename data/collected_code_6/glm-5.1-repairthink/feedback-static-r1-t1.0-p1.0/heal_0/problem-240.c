#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *next;
} Node;

Node *create_node(int val) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) {
        exit(EXIT_FAILURE);
    }
    n->val = val;
    n->next = NULL;
    return n;
}

Node *replace_last_with_list(Node *head, Node *new_list) {
    if (!new_list) {
        return head;
    }

    if (!head) {
        return new_list;
    }

    Node *current = head;
    while (current->next->next) {
        current = current->next;
    }

    free(current->next);
    current->next = new_list;

    return head;
}

void print_list(Node *head) {
    Node *current = head;
    while (current) {
        printf("%d ", current->val);
        current = current->next;
    }
    printf("\n");
}

void free_list(Node *head) {
    Node *current = head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    Node *list = create_node(1);
    list->next = create_node(2);
    list->next->next = create_node(3);

    Node *new_list = create_node(4);
    new_list->next = create_node(5);

    list = replace_last_with_list(list, new_list);
    print_list(list);

    free_list(list);

    return 0;
}