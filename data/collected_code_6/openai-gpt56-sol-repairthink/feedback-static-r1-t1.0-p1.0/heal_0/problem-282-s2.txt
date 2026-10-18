#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static void free_list(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
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

static int subtract_lists(const Node *first, const Node *second, Node **result)
{
    Node *head = NULL;
    Node *tail = NULL;
    int status;

    if (result == NULL) {
        return EINVAL;
    }

    *result = NULL;

    while (first != NULL) {
        if (!contains(second, first->value)) {
            status = append_node(&head, &tail, first->value);
            if (status != 0) {
                free_list(head);
                return status;
            }
        }
        first = first->next;
    }

    *result = head;
    return 0;
}

static int build_list(const int values[], size_t count, Node **result)
{
    Node *head = NULL;
    Node *tail = NULL;
    size_t i;
    int status;

    if (result == NULL || (values == NULL && count != 0U)) {
        return EINVAL;
    }

    *result = NULL;

    for (i = 0U; i < count; ++i) {
        status = append_node(&head, &tail, values[i]);
        if (status != 0) {
            free_list(head);
            return status;
        }
    }

    *result = head;
    return 0;
}

static int print_list(const Node *head)
{
    while (head != NULL) {
        if (printf("%d%s", head->value, head->next == NULL ? "\n" : " ") < 0) {
            return EIO;
        }
        head = head->next;
    }

    if (head == NULL && fflush(stdout) == EOF) {
        return EIO;
    }

    return 0;
}

int main(void)
{
    const int first_values[] = {1, 2, 3, 4, 5};
    const int second_values[] = {2, 4};
    Node *first = NULL;
    Node *second = NULL;
    Node *result = NULL;
    int status;

    status = build_list(first_values,
                        sizeof(first_values) / sizeof(first_values[0]),
                        &first);
    if (status != 0) {
        fprintf(stderr, "Failed to create first list: %d\n", status);
        return EXIT_FAILURE;
    }

    status = build_list(second_values,
                        sizeof(second_values) / sizeof(second_values[0]),
                        &second);
    if (status != 0) {
        fprintf(stderr, "Failed to create second list: %d\n", status);
        free_list(first);
        return EXIT_FAILURE;
    }

    status = subtract_lists(first, second, &result);
    if (status != 0) {
        fprintf(stderr, "Failed to subtract lists: %d\n", status);
        free_list(second);
        free_list(first);
        return EXIT_FAILURE;
    }

    status = print_list(result);

    free_list(result);
    free_list(second);
    free_list(first);

    if (status != 0) {
        fprintf(stderr, "Failed to print result: %d\n", status);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}