#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static Node *create_node(int value)
{
    Node *node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->value = value;
    node->next = NULL;
    return node;
}

static void free_list(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

static int append(Node **head, int value)
{
    Node *node;
    Node *tail;

    if (head == NULL) {
        return -1;
    }

    node = create_node(value);
    if (node == NULL) {
        return -1;
    }

    if (*head == NULL) {
        *head = node;
        return 0;
    }

    tail = *head;
    while (tail->next != NULL) {
        tail = tail->next;
    }

    tail->next = node;
    return 0;
}

static int replace_last_with_list(Node **head, Node **replacement)
{
    Node *previous;
    Node *last;

    if (head == NULL || replacement == NULL || *head == NULL) {
        return -1;
    }

    last = *head;
    previous = NULL;

    while (last->next != NULL) {
        previous = last;
        last = last->next;
    }

    if (previous == NULL) {
        *head = *replacement;
    } else {
        previous->next = *replacement;
    }

    *replacement = NULL;
    free(last);
    return 0;
}

static void print_list(const Node *head)
{
    const Node *current = head;

    while (current != NULL) {
        printf("%d", current->value);
        current = current->next;

        if (current != NULL) {
            printf(" ");
        }
    }

    printf("\n");
}

int main(void)
{
    Node *list = NULL;
    Node *replacement = NULL;
    int status = EXIT_FAILURE;

    if (append(&list, 1) != 0 ||
        append(&list, 2) != 0 ||
        append(&list, 3) != 0 ||
        append(&replacement, 4) != 0 ||
        append(&replacement, 5) != 0) {
        goto cleanup;
    }

    if (replace_last_with_list(&list, &replacement) != 0) {
        goto cleanup;
    }

    print_list(list);
    status = EXIT_SUCCESS;

cleanup:
    free_list(list);
    free_list(replacement);
    return status;
}