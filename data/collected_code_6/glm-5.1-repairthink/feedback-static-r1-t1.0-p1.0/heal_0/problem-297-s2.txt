#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int is_list;
    union {
        int value;
        struct Node *child;
    } data;
    struct Node *next;
} Node;

Node *create_value_node(int val) {
    Node *n = malloc(sizeof(Node));
    if (!n) {
        exit(EXIT_FAILURE);
    }
    n->is_list = 0;
    n->data.value = val;
    n->next = NULL;
    return n;
}

Node *create_list_node(Node *child) {
    Node *n = malloc(sizeof(Node));
    if (!n) {
        exit(EXIT_FAILURE);
    }
    n->is_list = 1;
    n->data.child = child;
    n->next = NULL;
    return n;
}

void append_node(Node **head, Node *node) {
    if (!*head) {
        *head = node;
    } else {
        Node *curr = *head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = node;
    }
}

Node *flatten(Node *head) {
    Node *result = NULL;
    Node **tail = &result;
    Node *curr = head;

    while (curr) {
        if (curr->is_list) {
            Node *flattened_child = flatten(curr->data.child);
            if (flattened_child) {
                *tail = flattened_child;
                while (*tail) {
                    tail = &((*tail)->next);
                }
            }
        } else {
            Node *new_node = create_value_node(curr->data.value);
            *tail = new_node;
            tail = &(new_node->next);
        }
        curr = curr->next;
    }
    return result;
}

void free_list(Node *head) {
    Node *curr = head;
    while (curr) {
        Node *next = curr->next;
        if (curr->is_list) {
            free_list(curr->data.child);
        }
        free(curr);
        curr = next;
    }
}

void print_list(Node *head) {
    Node *curr = head;
    while (curr) {
        printf("%d ", curr->data.value);
        curr = curr->next;
    }
    printf("\n");
}

int main() {
    Node *list1 = NULL;
    append_node(&list1, create_value_node(1));
    append_node(&list1, create_value_node(2));

    Node *list2 = NULL;
    append_node(&list2, create_value_node(3));
    append_node(&list2, create_value_node(4));

    Node *nested = NULL;
    append_node(&nested, create_value_node(0));
    append_node(&nested, create_list_node(list1));
    append_node(&nested, create_value_node(5));
    append_node(&nested, create_list_node(list2));
    append_node(&nested, create_value_node(6));

    Node *flat = flatten(nested);
    print_list(flat);

    free_list(nested);
    free_list(flat);

    return 0;
}