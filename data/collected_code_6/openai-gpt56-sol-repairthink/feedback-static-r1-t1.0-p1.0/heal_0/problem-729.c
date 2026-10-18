#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static void free_list(Node **head)
{
    Node *current;
    Node *next;

    if (head == NULL) {
        return;
    }

    current = *head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}

static int append_node(Node **head, Node **tail, int value)
{
    Node *node;

    if (head == NULL || tail == NULL) {
        return EINVAL;
    }

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return ENOMEM;
    }

    node->value = value;
    node->next = NULL;

    if (*tail == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    *tail = node;
    return 0;
}

static int add_lists(const Node *first, const Node *second, Node **result)
{
    Node *head = NULL;
    Node *tail = NULL;
    int status;

    if (result == NULL) {
        return EINVAL;
    }

    *result = NULL;

    while (first != NULL && second != NULL) {
        int sum;

        if ((second->value > 0 && first->value > INT_MAX - second->value) ||
            (second->value < 0 && first->value < INT_MIN - second->value)) {
            free_list(&head);
            return ERANGE;
        }

        sum = first->value + second->value;
        status = append_node(&head, &tail, sum);
        if (status != 0) {
            free_list(&head);
            return status;
        }

        first = first->next;
        second = second->next;
    }

    if (first != NULL || second != NULL) {
        free_list(&head);
        return EINVAL;
    }

    *result = head;
    return 0;
}

static int print_list(const Node *head)
{
    while (head != NULL) {
        if (printf("%d%s", head->value, head->next != NULL ? " " : "\n") < 0) {
            return EIO;
        }
        head = head->next;
    }

    return 0;
}

int main(void)
{
    const int first_values[] = {1, 2, 3, 4};
    const int second_values[] = {5, 6, 7, 8};
    const size_t count = sizeof(first_values) / sizeof(first_values[0]);
    Node *first = NULL;
    Node *first_tail = NULL;
    Node *second = NULL;
    Node *second_tail = NULL;
    Node *result = NULL;
    int status = 0;
    size_t i;

    for (i = 0; i < count; ++i) {
        status = append_node(&first, &first_tail, first_values[i]);
        if (status != 0) {
            goto cleanup;
        }

        status = append_node(&second, &second_tail, second_values[i]);
        if (status != 0) {
            goto cleanup;
        }
    }

    status = add_lists(first, second, &result);
    if (status != 0) {
        goto cleanup;
    }

    status = print_list(result);

cleanup:
    free_list(&result);
    free_list(&second);
    free_list(&first);

    if (status != 0) {
        if (fprintf(stderr, "Error: %s\n", strerror(status)) < 0) {
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}