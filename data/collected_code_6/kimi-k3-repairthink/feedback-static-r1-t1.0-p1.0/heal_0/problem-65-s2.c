#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

long long recursive_sum(const Node *head) {
    if (head == NULL) {
        return 0;
    }
    return (long long)head->data + recursive_sum(head->next);
}

Node* create_node(int value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    Node *head = NULL;
    Node *tail = NULL;
    int values[] = {10, 20, 30, 40, 50};
    size_t count = sizeof(values) / sizeof(values[0]);
    
    for (size_t i = 0; i < count; i++) {
        Node *new_node = create_node(values[i]);
        if (new_node == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            free_list(head);
            return EXIT_FAILURE;
        }
        
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    
    long long result = recursive_sum(head);
    
    if (result > INT_MAX || result < INT_MIN) {
        fprintf(stderr, "Error: Sum exceeds integer range\n");
        free_list(head);
        return EXIT_FAILURE;
    }
    
    printf("Sum: %lld\n", result);
    
    free_list(head);
    return EXIT_SUCCESS;
}