#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static size_t list_length(const Node *head)
{
    size_t length = 0;

    while (head != NULL) {
        if (length == SIZE_MAX) {
            fputs("List length overflow\n", stderr);
            exit(EXIT_FAILURE);
        }
        ++length;
        head = head->next;
    }

    return length;
}

static const Node *find_longest_list(const Node *const lists[],
                                     size_t list_count,
                                     size_t *max_length)
{
    const Node *longest = NULL;
    size_t longest_length = 0;

    if (max_length == NULL || (lists == NULL && list_count != 0U)) {
        return NULL;
    }

    for (size_t i = 0; i < list_count; ++i) {
        const size_t current_length = list_length(lists[i]);

        if (current_length > longest_length) {
            longest = lists[i];
            longest_length = current_length;
        }
    }

    *max_length = longest_length;
    return longest;
}

static Node *create_list(const int values[], size_t count)
{
    Node *head = NULL;
    Node **tail = &head;

    if (values == NULL && count != 0U) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        Node *node = malloc(sizeof(*node));

        if (node == NULL) {
            while (head != NULL) {
                Node *next = head->next;
                free(head);
                head = next;
            }
            return NULL;
        }

        node->value = values[i];
        node->next = NULL;
        *tail = node;
        tail = &node->next;
    }

    return head;
}

static void free_list(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void)
{
    const int values1[] = {1, 2, 3};
    const int values2[] = {4, 5, 6, 7, 8};
    const int values3[] = {9, 10};
    Node *lists[] = {
        create_list(values1, sizeof(values1) / sizeof(values1[0])),
        create_list(values2, sizeof(values2) / sizeof(values2[0])),
        create_list(values3, sizeof(values3) / sizeof(values3[0]))
    };
    const size_t list_count = sizeof(lists) / sizeof(lists[0]);
    size_t max_length = 0;
    const Node *longest = NULL;
    int status = EXIT_SUCCESS;

    for (size_t i = 0; i < list_count; ++i) {
        if (lists[i] == NULL) {
            fputs("Failed to create a list\n", stderr);
            status = EXIT_FAILURE;
            goto cleanup;
        }
    }

    longest = find_longest_list((const Node *const *)lists,
                                list_count,
                                &max_length);

    if (longest == NULL) {
        fputs("No non-empty list found\n", stderr);
        status = EXIT_FAILURE;
        goto cleanup;
    }

    printf("Maximum length: %zu\n", max_length);
    printf("Longest list:");

    for (const Node *node = longest; node != NULL; node = node->next) {
        printf(" %d", node->value);
    }

    putchar('\n');

cleanup:
    for (size_t i = 0; i < list_count; ++i) {
        free_list(lists[i]);
    }

    return status;
}