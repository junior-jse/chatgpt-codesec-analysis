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

static int append(Node **head, int value)
{
    Node *node;
    Node **current;

    if (head == NULL) {
        return 0;
    }

    node = create_node(value);
    if (node == NULL) {
        return 0;
    }

    current = head;
    while (*current != NULL) {
        current = &(*current)->next;
    }

    *current = node;
    return 1;
}

static int contains(const Node *head, int value)
{
    while (head != NULL) {
        if (head->value == value) {
            return 1;
        }
        head = head->next;
    }

    return 0;
}

static void remove_present(Node **list, const Node *values)
{
    Node **current;

    if (list == NULL) {
        return;
    }

    current = list;
    while (*current != NULL) {
        if (contains(values, (*current)->value)) {
            Node *removed = *current;
            *current = removed->next;
            free(removed);
        } else {
            current = &(*current)->next;
        }
    }
}

static void print_list(const Node *head)
{
    while (head != NULL) {
        printf("%d", head->value);
        if (head->next != NULL) {
            printf(" ");
        }
        head = head->next;
    }

    printf("\n");
}

static void free_list(Node **head)
{
    Node *current;

    if (head == NULL) {
        return;
    }

    current = *head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}

int main(void)
{
    const int first_values[] = {1, 2, 3, 4, 2, 5, 6};
    const int second_values[] = {2, 4, 6};
    const size_t first_count = sizeof(first_values) / sizeof(first_values[0]);
    const size_t second_count = sizeof(second_values) / sizeof(second_values[0]);
    Node *first = NULL;
    Node *second = NULL;
    size_t i;
    int status = EXIT_SUCCESS;

    for (i = 0; i < first_count; ++i) {
        if (!append(&first, first_values[i])) {
            status = EXIT_FAILURE;
            goto cleanup;
        }
    }

    for (i = 0; i < second_count; ++i) {
        if (!append(&second, second_values[i])) {
            status = EXIT_FAILURE;
            goto cleanup;
        }
    }

    remove_present(&first, second);
    print_list(first);

cleanup:
    free_list(&first);
    free_list(&second);

    if (status != EXIT_SUCCESS) {
        fputs("Memory allocation failed\n", stderr);
    }

    return status;
}