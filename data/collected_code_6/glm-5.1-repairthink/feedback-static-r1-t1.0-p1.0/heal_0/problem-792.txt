#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

size_t count_lists(Node **lists, size_t num_lists) {
    if (lists == NULL) {
        return 0;
    }

    size_t count = 0;
    for (size_t i = 0; i < num_lists; i++) {
        if (lists[i] != NULL) {
            count++;
        }
    }
    return count;
}

Node *create_node(int data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_lists(Node **lists, size_t num_lists) {
    if (lists == NULL) {
        return;
    }
    for (size_t i = 0; i < num_lists; i++) {
        Node *current = lists[i];
        while (current != NULL) {
            Node *next = current->next;
            free(current);
            current = next;
        }
    }
    free(lists);
}

int main(void) {
    size_t num_lists = 3;
    Node **lists = (Node **)malloc(num_lists * sizeof(Node *));
    if (lists == NULL) {
        return EXIT_FAILURE;
    }

    lists[0] = create_node(1);
    lists[0]->next = create_node(2);
    lists[1] = NULL;
    lists[2] = create_node(3);

    size_t result = count_lists(lists, num_lists);
    printf("%zu\n", result);

    free_lists(lists, num_lists);

    return 0;
}