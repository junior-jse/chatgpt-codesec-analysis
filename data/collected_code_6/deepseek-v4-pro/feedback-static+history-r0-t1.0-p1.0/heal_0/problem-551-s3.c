#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int *data;
    int size;
    struct Node *next;
} Node;

int *extract_column(Node *list, int col) {
    if (list == NULL || col < 0) {
        return NULL;
    }

    int count = 0;
    Node *current = list;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    int *result = (int *)malloc(count * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    current = list;
    int index = 0;
    while (current != NULL) {
        if (col >= current->size) {
            free(result);
            return NULL;
        }
        result[index++] = current->data[col];
        current = current->next;
    }

    return result;
}

Node *create_node(int *data, int size) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->size = size;
    node->next = NULL;
    return node;
}

void append_node(Node **head, Node *node) {
    if (*head == NULL) {
        *head = node;
        return;
    }
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = node;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

int main() {
    int row1[] = {1, 2, 3};
    int row2[] = {4, 5, 6};
    int row3[] = {7, 8, 9};

    Node *list = NULL;
    append_node(&list, create_node(row1, 3));
    append_node(&list, create_node(row2, 3));
    append_node(&list, create_node(row3, 3));

    int *column = extract_column(list, 1);
    if (column != NULL) {
        for (int i = 0; i < 3; i++) {
            printf("%d ", column[i]);
        }
        printf("\n");
        free(column);
    }

    free_list(list);
    return 0;
}