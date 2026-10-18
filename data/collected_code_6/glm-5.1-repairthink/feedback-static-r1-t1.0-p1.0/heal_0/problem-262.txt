#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Node *create_node(int data) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

void split_list(struct Node *source, int k, struct Node **front, struct Node **back) {
    *front = NULL;
    *back = NULL;

    if (source == NULL) {
        return;
    }

    if (k <= 0) {
        *back = source;
        return;
    }

    *front = source;
    struct Node *current = source;

    for (int i = 1; i < k && current->next != NULL; i++) {
        current = current->next;
    }

    *back = current->next;
    current->next = NULL;
}

void free_list(struct Node *head) {
    struct Node *current = head;
    struct Node *next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}

int main() {
    struct Node *source = create_node(1);
    source->next = create_node(2);
    source->next->next = create_node(3);
    source->next->next->next = create_node(4);
    source->next->next->next->next = create_node(5);

    struct Node *front = NULL;
    struct Node *back = NULL;

    split_list(source, 2, &front, &back);

    printf("Front:");
    struct Node *curr = front;
    while (curr != NULL) {
        printf(" %d", curr->data);
        curr = curr->next;
    }
    printf("\n");

    printf("Back:");
    curr = back;
    while (curr != NULL) {
        printf(" %d", curr->data);
        curr = curr->next;
    }
    printf("\n");

    free_list(front);
    free_list(back);

    return 0;
}