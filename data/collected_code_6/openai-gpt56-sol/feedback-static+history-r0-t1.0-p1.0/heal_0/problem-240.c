#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static Node *node_create(int value)
{
    Node *node = malloc(sizeof(*node));

    if (node == NULL) {
        return NULL;
    }

    node->value = value;
    node->next = NULL;
    return node;
}

static void list_free(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

static int list_append(Node **head, int value)
{
    Node *node;
    Node *current;

    if (head == NULL) {
        return 0;
    }

    node = node_create(value);
    if (node == NULL) {
        return 0;
    }

    if (*head == NULL) {
        *head = node;
        return 1;
    }

    current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = node;
    return 1;
}

static int list_replace_last(Node **head, Node **replacement)
{
    Node *current;

    if (head == NULL || replacement == NULL || *head == NULL) {
        return 0;
    }

    if ((*head)->next == NULL) {
        free(*head);
        *head = *replacement;
        *replacement = NULL;
        return 1;
    }

    current = *head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    free(current->next);
    current->next = *replacement;
    *replacement = NULL;
    return 1;
}

static void list_print(const Node *head)
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

    if (!list_append(&list, 1) ||
        !list_append(&list, 2) ||
        !list_append(&list, 3) ||
        !list_append(&replacement, 4) ||
        !list_append(&replacement, 5)) {
        goto cleanup;
    }

    if (!list_replace_last(&list, &replacement)) {
        goto cleanup;
    }

    list_print(list);
    status = EXIT_SUCCESS;

cleanup:
    list_free(list);
    list_free(replacement);
    return status;
}