#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node* next;
    struct Node* child;
} Node;

Node* create_node(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    node->child = NULL;
    return node;
}

void free_list(Node* head) {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        if (current->child) {
            free_list(current->child);
        }
        free(current);
        current = next;
    }
}

bool contains(Node* list, int value) {
    Node* current = list;
    while (current) {
        if (current->data == value) {
            return true;
        }
        if (current->child && contains(current->child, value)) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void find_common_elements(Node* list1, Node* list2, Node** result) {
    Node* current = list1;
    while (current) {
        if (contains(list2, current->data)) {
            Node* new_node = create_node(current->data);
            new_node->next = *result;
            *result = new_node;
        }
        if (current->child) {
            find_common_elements(current->child, list2, result);
        }
        current = current->next;
    }
}

void print_list(Node* head) {
    Node* current = head;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    Node* list1 = create_node(1);
    list1->next = create_node(2);
    list1->next->next = create_node(3);
    list1->next->child = create_node(4);
    list1->next->child->next = create_node(5);

    Node* list2 = create_node(3);
    list2->next = create_node(4);
    list2->next->next = create_node(7);

    Node* result = NULL;
    find_common_elements(list1, list2, &result);
    print_list(result);

    free_list(list1);
    free_list(list2);
    free_list(result);

    return 0;
}