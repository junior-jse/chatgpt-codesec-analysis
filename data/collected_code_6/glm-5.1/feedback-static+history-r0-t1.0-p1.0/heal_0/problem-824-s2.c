#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Node *remove_even(struct Node *head) {
    struct Node **indirect = &head;
    while (*indirect) {
        struct Node *current = *indirect;
        if (current->data % 2 == 0) {
            *indirect = current->next;
            free(current);
        } else {
            indirect = &current->next;
        }
    }
    return head;
}

struct Node *create_node(int data) {
    struct Node *new_node = malloc(sizeof(struct Node));
    if (!new_node) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_list(struct Node *head) {
    struct Node *current = head;
    while (current) {
        struct Node *next = current->next;
        free(current);
        current = next;
    }
}

void print_list(struct Node *head) {
    struct Node *current = head;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    struct Node *head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);
    head->next->next->next->next = create_node(5);
    
    print_list(head);
    head = remove_even(head);
    print_list(head);
    
    free_list(head);
    head = NULL;
    
    return 0;
}