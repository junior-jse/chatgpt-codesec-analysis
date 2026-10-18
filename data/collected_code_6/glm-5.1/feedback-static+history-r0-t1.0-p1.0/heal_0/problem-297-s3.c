#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    bool is_list;
    union {
        int value;
        struct {
            struct Node* head;
            struct Node* tail;
        } list;
    };
} Node;

Node* create_int_node(int val) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->is_list = false;
    node->value = val;
    return node;
}

Node* create_list_node(Node* head, Node* tail) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->is_list = true;
    node->list.head = head;
    node->list.tail = tail;
    return node;
}

typedef struct IntNode {
    int value;
    struct IntNode* next;
} IntNode;

IntNode* create_flat_node(int val) {
    IntNode* node = (IntNode*)malloc(sizeof(IntNode));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->value = val;
    node->next = NULL;
    return node;
}

void free_nested(Node* node) {
    if (node == NULL) {
        return;
    }
    if (node->is_list) {
        free_nested(node->list.head);
        free_nested(node->list.tail);
    }
    free(node);
}

void free_flat(IntNode* node) {
    while (node != NULL) {
        IntNode* next = node->next;
        free(node);
        node = next;
    }
}

void flatten_helper(Node* node, IntNode** tail) {
    if (node == NULL) {
        return;
    }
    if (node->is_list) {
        flatten_helper(node->list.head, tail);
        flatten_helper(node->list.tail, tail);
    } else {
        IntNode* new_node = create_flat_node(node->value);
        (*tail)->next = new_node;
        *tail = new_node;
    }
}

IntNode* flatten(Node* root) {
    if (root == NULL) {
        return NULL;
    }

    IntNode dummy;
    dummy.next = NULL;
    IntNode* tail = &dummy;

    flatten_helper(root, &tail);

    IntNode* result = dummy.next;
    return result;
}

int main(void) {
    Node* n1 = create_int_node(1);
    Node* n2 = create_int_node(2);
    Node* n3 = create_int_node(3);
    Node* n4 = create_int_node(4);
    Node* n5 = create_int_node(5);

    Node* inner_list = create_list_node(n2, create_list_node(n3, NULL));
    Node* outer_list = create_list_node(n1, create_list_node(inner_list, create_list_node(n4, create_list_node(n5, NULL))));

    IntNode* flat = flatten(outer_list);

    IntNode* curr = flat;
    while (curr != NULL) {
        printf("%d ", curr->value);
        curr = curr->next;
    }
    printf("\n");

    free_flat(flat);
    free_nested(outer_list);

    return 0;
}