#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int value) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        return NULL;
    }
    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}

void free_list(Node *head) {
    Node *current = head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

Node *map_power(int bases[], int size) {
    if (!bases || size <= 0) {
        return NULL;
    }

    Node *head = NULL;
    Node *tail = NULL;

    for (int i = 0; i < size; i++) {
        int value = power(bases[i], i);
        Node *new_node = create_node(value);
        if (!new_node) {
            free_list(head);
            return NULL;
        }
        if (!head) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    return head;
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
    int bases[] = {2, 3, 4, 5, 6};
    int size = sizeof(bases) / sizeof(bases[0]);

    Node *list = map_power(bases, size);
    if (!list) {
        fprintf(stderr, "Failed to create list\n");
        return 1;
    }

    print_list(list);
    free_list(list);

    return 0;
}