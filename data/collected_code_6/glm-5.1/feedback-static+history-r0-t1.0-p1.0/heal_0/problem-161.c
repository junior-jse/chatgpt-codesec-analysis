#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

Node *append_node(Node *head, int data) {
    Node *node = create_node(data);
    if (!head) {
        return node;
    }
    Node *current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = node;
    return head;
}

bool is_present(Node *head, int value) {
    Node *current = head;
    while (current) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

Node *remove_elements(Node *list, Node *remove_list) {
    Node dummy;
    dummy.next = list;
    Node *prev = &dummy;
    Node *current = list;

    while (current) {
        if (is_present(remove_list, current->data)) {
            prev->next = current->next;
            free(current);
            current = prev->next;
        } else {
            prev = current;
            current = current->next;
        }
    }

    return dummy.next;
}

void free_list(Node *head) {
    Node *current = head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

void print_list(Node *head) {
    Node *current = head;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    Node *list = NULL;
    list = append_node(list, 1);
    list = append_node(list, 2);
    list = append_node(list, 3);
    list = append_node(list, 4);
    list = append_node(list, 5);

    Node *remove_list = NULL;
    remove_list = append_node(remove_list, 2);
    remove_list = append_node(remove_list, 4);

    printf("Original list: ");
    print_list(list);

    printf("Remove list: ");
    print_list(remove_list);

    list = remove_elements(list, remove_list);

    printf("Result list: ");
    print_list(list);

    free_list(list);
    free_list(remove_list);

    return 0;
}