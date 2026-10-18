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
    Node *n = malloc(sizeof(Node));
    if (n == NULL) {
        exit(EXIT_FAILURE);
    }
    n->data = data;
    n->next = NULL;
    return n;
}

void free_lists(Node **lists, size_t num_lists) {
    if (lists == NULL) {
        return;
    }
    for (size_t i = 0; i < num_lists; i++) {
        Node *curr = lists[i];
        while (curr != NULL) {
            Node *next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(lists);
}

int main(void) {
    size_t num_lists = 4;
    Node **lists = malloc(num_lists * sizeof(Node *));
    if (lists == NULL) {
        return EXIT_FAILURE;
    }

    lists[0] = create_node(1);
    lists[1] = NULL;
    lists[2] = create_node(2);
    lists[2]->next = create_node(3);
    lists[3] = NULL;

    size_t count = count_lists(lists, num_lists);
    printf("%zu\n", count);

    free_lists(lists, num_lists);

    return 0;
}