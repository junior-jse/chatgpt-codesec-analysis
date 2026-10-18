#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int *data;
    size_t size;
    struct Node *next;
} Node;

bool lists_equal(const int *a, size_t a_size, const int *b, size_t b_size) {
    if (a_size != b_size) {
        return false;
    }
    for (size_t i = 0; i < a_size; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

Node *remove_duplicates(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *runner = current;
        while (runner->next != NULL) {
            if (lists_equal(current->data, current->size, runner->next->data, runner->next->size)) {
                Node *temp = runner->next;
                runner->next = runner->next->next;
                free(temp->data);
                free(temp);
            } else {
                runner = runner->next;
            }
        }
        current = current->next;
    }
    return head;
}

Node *create_node(const int *arr, size_t size) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->size = size;
    new_node->data = malloc(size * sizeof(int));
    if (new_node->data == NULL) {
        free(new_node);
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < size; ++i) {
        new_node->data[i] = arr[i];
    }
    new_node->next = NULL;
    return new_node;
}

Node *append_node(Node *head, const int *arr, size_t size) {
    Node *new_node = create_node(arr, size);
    if (head == NULL) {
        return new_node;
    }
    Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return head;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("[");
        for (size_t i = 0; i < current->size; ++i) {
            printf("%d", current->data[i]);
            if (i < current->size - 1) {
                printf(", ");
            }
        }
        printf("]\n");
        current = current->next;
    }
}

int main(void) {
    Node *head = NULL;
    
    int a[] = {1, 2, 3};
    int b[] = {4, 5};
    int c[] = {1, 2, 3};
    int d[] = {7, 8, 9};
    int e[] = {4, 5};
    
    head = append_node(head, a, sizeof(a) / sizeof(a[0]));
    head = append_node(head, b, sizeof(b) / sizeof(b[0]));
    head = append_node(head, c, sizeof(c) / sizeof(c[0]));
    head = append_node(head, d, sizeof(d) / sizeof(d[0]));
    head = append_node(head, e, sizeof(e) / sizeof(e[0]));
    
    printf("Original:\n");
    print_list(head);
    
    head = remove_duplicates(head);
    
    printf("\nAfter removing duplicates:\n");
    print_list(head);
    
    free_list(head);
    
    return 0;
}