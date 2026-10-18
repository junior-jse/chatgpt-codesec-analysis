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

int select_nth(Node *head, int n) {
    if (head == NULL || n < 0) {
        return -1;
    }
    Node *current = head;
    int index = 0;
    while (current != NULL) {
        if (index == n) {
            return current->data;
        }
        index++;
        current = current->next;
    }
    return -1;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

int main(void) {
    Node *list = NULL;
    append_node(&list, 10);
    append_node(&list, 20);
    append_node(&list, 30);
    append_node(&list, 40);
    append_node(&list, 50);

    int result = select_nth(list, 2);
    if (result != -1) {
        printf("%d\n", result);
    }

    result = select_nth(list, 5);
    if (result != -1) {
        printf("%d\n", result);
    }

    result = select_nth(list, -1);
    if (result != -1) {
        printf("%d\n", result);
    }

    free_list(list);
    return 0;
}