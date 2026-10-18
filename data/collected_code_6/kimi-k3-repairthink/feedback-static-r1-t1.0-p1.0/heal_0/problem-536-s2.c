#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

static Node *create_node(int data)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

static void append_node(Node **head, int data)
{
    Node *new_node = create_node(data);
    Node *current;

    if (new_node == NULL) {
        return;
    }

    if (*head == NULL) {
        *head = new_node;
        return;
    }

    current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

static int get_nth_item(const Node *head, size_t n, int *result)
{
    const Node *current = head;
    size_t index = 0;

    if (result == NULL) {
        return -1;
    }

    while (current != NULL) {
        if (index == n) {
            *result = current->data;
            return 0;
        }
        current = current->next;
        index++;
    }

    return -1;
}

static void free_list(Node *head)
{
    Node *current = head;
    Node *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main(void)
{
    Node *list = NULL;
    int result = 0;
    size_t n = 2;
    int i;

    for (i = 0; i < 5; i++) {
        append_node(&list, (i + 1) * 10);
    }

    if (get_nth_item(list, n, &result) == 0) {
        printf("Item at index %zu: %d\n", n, result);
    } else {
        fprintf(stderr, "Error: Index %zu out of bounds\n", n);
    }

    free_list(list);
    list = NULL;

    return 0;
}