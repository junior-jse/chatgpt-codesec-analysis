#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    size_t length;
} List;

static void list_init(List *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

static int list_append(List *list, int value)
{
    Node *node = malloc(sizeof(*node));

    if (node == NULL) {
        return 0;
    }

    node->value = value;
    node->next = NULL;

    if (list->tail == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
    }

    list->tail = node;
    list->length++;
    return 1;
}

static void list_destroy(List *list)
{
    Node *current = list->head;

    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }

    list_init(list);
}

static const List *find_longest_sublist(const List *lists, size_t count)
{
    const List *longest;

    if (lists == NULL || count == 0) {
        return NULL;
    }

    longest = &lists[0];

    for (size_t i = 1; i < count; i++) {
        if (lists[i].length > longest->length) {
            longest = &lists[i];
        }
    }

    return longest;
}

static void list_print(const List *list)
{
    const Node *current = list->head;

    while (current != NULL) {
        printf("%d", current->value);

        if (current->next != NULL) {
            printf(" ");
        }

        current = current->next;
    }

    printf("\n");
}

int main(void)
{
    int values[][5] = {
        {1, 2, 0, 0, 0},
        {3, 4, 5, 6, 7},
        {8, 9, 10, 0, 0}
    };
    const size_t lengths[] = {2, 5, 3};
    const size_t count = sizeof(lengths) / sizeof(lengths[0]);
    List *lists = calloc(count, sizeof(*lists));
    const List *longest = NULL;
    int status = EXIT_FAILURE;

    if (lists == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; i++) {
        list_init(&lists[i]);

        for (size_t j = 0; j < lengths[i]; j++) {
            if (!list_append(&lists[i], values[i][j])) {
                fprintf(stderr, "Memory allocation failed\n");
                goto cleanup;
            }
        }
    }

    longest = find_longest_sublist(lists, count);

    if (longest == NULL) {
        fprintf(stderr, "No sublists available\n");
        goto cleanup;
    }

    printf("Maximum length: %zu\n", longest->length);
    printf("Sublist: ");
    list_print(longest);
    status = EXIT_SUCCESS;

cleanup:
    for (size_t i = 0; i < count; i++) {
        list_destroy(&lists[i]);
    }

    free(lists);
    return status;
}