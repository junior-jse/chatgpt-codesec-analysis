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
        ++length;
        head = head->next;
    }

    return length;
}

/* Possible weaknesses found:
 *  Parameter 'lists' can be declared as const array [constParameter]
 */
static const Node *find_longest_list(Node *const lists[], size_t count, size_t *max_length)
{
    const Node *longest = NULL;
    size_t longest_length = 0;

    if (max_length == NULL || (lists == NULL && count != 0)) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        size_t length = list_length(lists[i]);

        if (longest == NULL || length > longest_length) {
            longest = lists[i];
            longest_length = length;
        }
    }

    *max_length = longest_length;
    return longest;
}

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
        return 0;
    }

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return 0;
    }

    node->value = value;
    node->next = NULL;

    if (*tail == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    *tail = node;
    return 1;
}

int main(void)
{
    Node *lists[3] = {NULL, NULL, NULL};
    Node *tails[3] = {NULL, NULL, NULL};
    const Node *longest;
    size_t max_length;
    int values[][4] = {
        {1, 2, 0, 0},
        {3, 4, 5, 6},
        {7, 8, 9, 0}
    };
    /* Possible weaknesses found:
     *  Variable 'lengths' can be declared as const array [constVariable]
     */
    size_t lengths[] = {2, 4, 3};
    int status = EXIT_SUCCESS;

    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < lengths[i]; ++j) {
            if (!append_node(&lists[i], &tails[i], values[i][j])) {
                fprintf(stderr, "Memory allocation failed.\n");
                status = EXIT_FAILURE;
                goto cleanup;
            }
        }
    }

    longest = find_longest_list(lists, 3, &max_length);
    if (longest == NULL) {
        fprintf(stderr, "No non-empty list found.\n");
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
    for (size_t i = 0; i < 3; ++i) {
        free_list(lists[i]);
    }

    return status;
}