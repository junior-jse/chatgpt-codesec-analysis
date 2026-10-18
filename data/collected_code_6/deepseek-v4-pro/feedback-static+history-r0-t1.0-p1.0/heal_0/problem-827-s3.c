#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int *data;
    int length;
    struct Node *next;
} Node;

int sum_column(Node *head, int col) {
    if (head == NULL || col < 0) return 0;
    int sum = 0;
    int valid = 0;
    Node *current = head;
    while (current != NULL) {
        if (current->data != NULL && col < current->length) {
            sum += current->data[col];
            valid = 1;
        }
        current = current->next;
    }
    return valid ? sum : 0;
}

Node *create_list(void) {
    Node *head = NULL;
    Node *tail = NULL;
    int sizes[] = {3, 3, 3};
    int values[][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    for (int i = 0; i < 3; i++) {
        Node *new_node = (Node *)malloc(sizeof(Node));
        if (new_node == NULL) {
            Node *temp = head;
            while (temp != NULL) {
                Node *next = temp->next;
                free(temp->data);
                free(temp);
                temp = next;
            }
            return NULL;
        }
        new_node->length = sizes[i];
        new_node->data = (int *)malloc(sizeof(int) * sizes[i]);
        if (new_node->data == NULL) {
            free(new_node);
            Node *temp = head;
            while (temp != NULL) {
                Node *next = temp->next;
                free(temp->data);
                free(temp);
                temp = next;
            }
            return NULL;
        }
        for (int j = 0; j < sizes[i]; j++) {
            new_node->data[j] = values[i][j];
        }
        new_node->next = NULL;
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    return head;
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

int main(void) {
    Node *list = create_list();
    if (list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    int result = sum_column(list, 1);
    printf("%d\n", result);
    free_list(list);
    return EXIT_SUCCESS;
}