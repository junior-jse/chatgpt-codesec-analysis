#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int *data;
    int size;
    struct Node *next;
} Node;

int sum_column(Node *head, int column_index) {
    if (head == NULL || column_index < 0) {
        return 0;
    }

    int sum = 0;
    Node *current = head;

    while (current != NULL) {
        if (column_index < current->size) {
            sum += current->data[column_index];
        }
        current = current->next;
    }

    return sum;
}

Node *create_node(int *data, int size) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->data = data;
    new_node->size = size;
    new_node->next = NULL;

    return new_node;
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
    int row1[] = {1, 2, 3};
    int row2[] = {4, 5, 6, 7};
    int row3[] = {8, 9};

    Node *head = create_node(row1, 3);
    if (head == NULL) {
        return 1;
    }

    Node *node2 = create_node(row2, 4);
    if (node2 == NULL) {
        free_list(head);
        return 1;
    }
    head->next = node2;

    Node *node3 = create_node(row3, 2);
    if (node3 == NULL) {
        free_list(head);
        return 1;
    }
    node2->next = node3;

    int column_sum = sum_column(head, 1);
    printf("%d\n", column_sum);

    free_list(head);

    return 0;
}