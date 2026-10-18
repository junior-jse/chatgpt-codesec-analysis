#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

Node *replace_last_with_list(Node *head, Node *new_list) {
    if (new_list == NULL) {
        return head;
    }
    if (head == NULL) {
        return new_list;
    }
    Node *current = head;
    Node *prev = NULL;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }
    if (prev != NULL) {
        prev->next = new_list;
    } else {
        head = new_list;
    }
    free(current);
    return head;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    Node *list1 = create_node(1);
    list1->next = create_node(2);
    list1->next->next = create_node(3);

    Node *list2 = create_node(4);
    list2->next = create_node(5);

    printf("List 1: ");
    print_list(list1);
    printf("List 2: ");
    print_list(list2);

    list1 = replace_last_with_list(list1, list2);

    printf("Result: ");
    print_list(list1);

    free_list(list1);

    return 0;
}