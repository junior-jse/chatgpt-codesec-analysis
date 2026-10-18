#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void append_node(Node **head, int data) {
    Node *new_node = create_node(data);
    if (new_node == NULL) {
        return;
    }
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

int split_list(Node *head, int first_len, Node **first_part, Node **second_part) {
    if (first_len < 0 || first_part == NULL || second_part == NULL) {
        return -1;
    }
    
    *first_part = NULL;
    *second_part = NULL;
    
    if (head == NULL) {
        return 0;
    }
    
    if (first_len == 0) {
        *second_part = head;
        return 0;
    }
    
    Node *current = head;
    int count = 1;
    
    while (count < first_len && current->next != NULL) {
        current = current->next;
        count++;
    }
    
    if (count < first_len) {
        return -1;
    }
    
    *first_part = head;
    *second_part = current->next;
    current->next = NULL;
    
    return 0;
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

void free_list(Node *head) {
    Node *current = head;
    Node *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main(void) {
    Node *list = NULL;
    Node *first_part = NULL;
    Node *second_part = NULL;
    
    append_node(&list, 1);
    append_node(&list, 2);
    append_node(&list, 3);
    append_node(&list, 4);
    append_node(&list, 5);
    
    printf("Original list: ");
    print_list(list);
    
    int split_len = 2;
    int result = split_list(list, split_len, &first_part, &second_part);
    
    if (result == 0) {
        printf("First part (length %d): ", split_len);
        print_list(first_part);
        printf("Second part: ");
        print_list(second_part);
        
        free_list(first_part);
        free_list(second_part);
    } else {
        printf("Split failed: invalid length\n");
        free_list(list);
    }
    
    return 0;
}