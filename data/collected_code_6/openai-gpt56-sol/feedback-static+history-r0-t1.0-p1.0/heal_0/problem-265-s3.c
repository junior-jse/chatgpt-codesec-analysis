#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} List;

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

static int append_node(List *list, int value)
{
    Node *node;

    if (list == NULL) {
        return EINVAL;
    }

    node = create_node(value);
    if (node == NULL) {
        return ENOMEM;
    }

    if (list->tail == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
    }

    list->tail = node;
    return 0;
}

static void free_list(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

static int split_every_nth(Node *head, size_t n, List **parts_out,
                           size_t *part_count_out)
{
    size_t node_count = 0;
    size_t part_count;
    size_t part_index = 0;
    size_t position = 0;
    Node *current;
    List *parts;

    if (parts_out == NULL || part_count_out == NULL || n == 0U) {
        return EINVAL;
    }

    *parts_out = NULL;
    *part_count_out = 0U;

    for (current = head; current != NULL; current = current->next) {
        if (node_count == SIZE_MAX) {
            return EOVERFLOW;
        }
        ++node_count;
    }

    if (node_count == 0U) {
        return 0;
    }

    part_count = node_count / n;
    if (node_count % n != 0U) {
        ++part_count;
    }

    if (part_count > SIZE_MAX / sizeof(*parts)) {
        return EOVERFLOW;
    }

    parts = calloc(part_count, sizeof(*parts));
    if (parts == NULL) {
        return ENOMEM;
    }

    current = head;
    while (current != NULL) {
        Node *next = current->next;

        current->next = NULL;

        if (parts[part_index].tail == NULL) {
            parts[part_index].head = current;
        } else {
            parts[part_index].tail->next = current;
        }
        parts[part_index].tail = current;

        ++position;
        if (position == n && next != NULL) {
            position = 0U;
            ++part_index;
        }

        current = next;
    }

    *parts_out = parts;
    *part_count_out = part_count;
    return 0;
}

static void print_list(const Node *head)
{
    const Node *current = head;

    putchar('[');
    while (current != NULL) {
        printf("%d", current->value);
        current = current->next;
        if (current != NULL) {
            printf(", ");
        }
    }
    puts("]");
}

int main(void)
{
    static const int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const size_t n = 3U;
    List source = {NULL, NULL};
    List *parts = NULL;
    size_t part_count = 0U;
    size_t i;
    int status;

    for (i = 0U; i < sizeof(values) / sizeof(values[0]); ++i) {
        status = append_node(&source, values[i]);
        if (status != 0) {
            fprintf(stderr, "Failed to create list: %d\n", status);
            free_list(source.head);
            return EXIT_FAILURE;
        }
    }

    status = split_every_nth(source.head, n, &parts, &part_count);
    if (status != 0) {
        fprintf(stderr, "Failed to split list: %d\n", status);
        free_list(source.head);
        return EXIT_FAILURE;
    }

    source.head = NULL;
    source.tail = NULL;

    for (i = 0U; i < part_count; ++i) {
        print_list(parts[i].head);
    }

    for (i = 0U; i < part_count; ++i) {
        free_list(parts[i].head);
    }
    free(parts);

    return EXIT_SUCCESS;
}