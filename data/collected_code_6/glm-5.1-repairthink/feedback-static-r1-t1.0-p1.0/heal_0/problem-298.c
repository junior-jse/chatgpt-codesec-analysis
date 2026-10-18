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

Node *create_int_node(int val) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) exit(EXIT_FAILURE);
    n->is_list = 0;
    n->data.value = val;
    n->next = NULL;
    return n;
}

Node *create_list_node(Node *child) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) exit(EXIT_FAILURE);
    n->is_list = 1;
    n->data.child = child;
    n->next = NULL;
    return n;
}

void free_list(Node *head) {
    Node *current = head;
    while (current) {
        Node *next = current->next;
        if (current->is_list) {
            free_list(current->data.child);
        }
        free(current);
        current = next;
    }
}

int int_in_list(Node *head, int val) {
    Node *current = head;
    while (current) {
        if (!current->is_list && current->data.value == val) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

Node *find_nested_elements(Node *nested, Node *flat) {
    Node *result = NULL;
    Node *tail = NULL;
    Node *current = nested;

    while (current) {
        if (current->is_list) {
            Node *child = current->data.child;
            while (child) {
                if (!child->is_list) {
                    if (int_in_list(flat, child->data.value)) {
                        Node *new_node = create_int_node(child->data.value);
                        if (!result) {
                            result = new_node;
                            tail = new_node;
                        } else {
                            tail->next = new_node;
                            tail = new_node;
                        }
                    }
                }
                child = child->next;
            }
        } else {
            if (int_in_list(flat, current->data.value)) {
                Node *new_node = create_int_node(current->data.value);
                if (!result) {
                    result = new_node;
                    tail = new_node;
                } else {
                    tail->next = new_node;
                    tail = new_node;
                }
            }
        }
        current = current->next;
    }
    return result;
}

void print_list(Node *head) {
    Node *current = head;
    while (current) {
        if (current->is_list) {
            printf("[");
            print_list(current->data.child);
            printf("]");
        } else {
            printf("%d", current->data.value);
        }
        if (current->next) {
            printf(", ");
        }
        current = current->next;
    }
}

int main(void) {
    Node *inner1 = create_int_node(1);
    inner1->next = create_int_node(2);
    inner1->next->next = create_int_node(3);

    Node *inner2 = create_int_node(4);
    inner2->next = create_int_node(5);

    Node *nested = create_list_node(inner1);
    nested->next = create_int_node(6);
    nested->next->next = create_list_node(inner2);
    nested->next->next->next = create_int_node(7);

    Node *flat = create_int_node(2);
    flat->next = create_int_node(4);
    flat->next->next = create_int_node(7);
    flat->next->next->next = create_int_node(9);

    Node *result = find_nested_elements(nested, flat);

    printf("Nested: ");
    print_list(nested);
    printf("\n");

    printf("Flat: ");
    print_list(flat);
    printf("\n");

    printf("Intersection: ");
    print_list(result);
    printf("\n");

    free_list(nested);
    free_list(flat);
    free_list(result);

    return 0;
}