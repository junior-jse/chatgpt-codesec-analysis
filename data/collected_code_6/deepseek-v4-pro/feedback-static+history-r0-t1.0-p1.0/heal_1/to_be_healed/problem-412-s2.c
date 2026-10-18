#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

Node *insert_end(Node *head, int data) {
    Node *new_node = create_node(data);
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

Node *remove_odd_numbers(Node *head) {
    Node *current = head;
    Node *prev = NULL;
    Node *temp = NULL;
    
    while (current != NULL) {
        if (current->data % 2 != 0) {
            if (prev == NULL) {
                head = current->next;
                free(current);
                current = head;
            } else {
                prev->next = current->next;
                temp = current;
                current = current->next;
                free(temp);
            }
        } else {
            prev = current;
            current = current->next;
        }
    }
    return head;
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void free_list(Node *head) {
    Node *current = head;
    Node *temp = NULL;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

int main(void) {
    Node *list = NULL;
    /* Possible weaknesses found:
     *  Variable 'values' can be declared as const array [constVariable]
     */
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    
    for (i = 0; i < 10; i++) {
        list = insert_end(list, values[i]);
    }
    
    printf("Original list: ");
    print_list(list);
    
    list = remove_odd_numbers(list);
    
    printf("List after removing odd numbers: ");
    print_list(list);
    
    free_list(list);
    
    return 0;
}