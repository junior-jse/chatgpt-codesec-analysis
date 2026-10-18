#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int *data;
    int size;
    struct Node *next;
} Node;

Node *remove_out_of_range(Node *head, int min_val, int max_val) {
    Node *current = head;
    Node *prev = NULL;
    Node *new_head = head;

    while (current != NULL) {
        int valid = 1;
        if (current->size > 0 && current->data != NULL) {
            for (int i = 0; i < current->size; i++) {
                if (current->data[i] < min_val || current->data[i] > max_val) {
                    valid = 0;
                    break;
                }
            }
        } else {
            valid = 0;
        }

        if (!valid) {
            Node *to_delete = current;
            if (prev == NULL) {
                new_head = current->next;
            } else {
                prev->next = current->next;
            }
            current = current->next;
            free(to_delete->data);
            free(to_delete);
        } else {
            prev = current;
            current = current->next;
        }
    }
    return new_head;
}

Node *create_sublist(int *arr, int size) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) return NULL;
    node->data = (int *)malloc(size * sizeof(int));
    if (node->data == NULL) {
        free(node);
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        node->data[i] = arr[i];
    }
    node->size = size;
    node->next = NULL;
    return node;
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
    int a1[] = {1, 2, 3};
    int a2[] = {4, 5, 6};
    int a3[] = {7, 8, 9};
    int a4[] = {10, 2, 11};

    Node *n1 = create_sublist(a1, 3);
    Node *n2 = create_sublist(a2, 3);
    Node *n3 = create_sublist(a3, 3);
    Node *n4 = create_sublist(a4, 3);

    if (n1 == NULL || n2 == NULL || n3 == NULL || n4 == NULL) {
        free_list(n1);
        free_list(n2);
        free_list(n3);
        free_list(n4);
        return 1;
    }

    n1->next = n2;
    n2->next = n3;
    n3->next = n4;

    Node *head = n1;

    int min_val = 1;
    int max_val = 6;

    head = remove_out_of_range(head, min_val, max_val);

    Node *curr = head;
    while (curr != NULL) {
        for (int i = 0; i < curr->size; i++) {
            printf("%d ", curr->data[i]);
        }
        printf("\n");
        curr = curr->next;
    }

    free_list(head);
    return 0;
}