#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    struct List *next;
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

static List *create_list(void)
{
    List *list = malloc(sizeof(*list));

    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->next = NULL;
    return list;
}

static int append_value(List *list, int value)
{
    Node *node;
    Node **current;

    if (list == NULL) {
        return 0;
    }

    node = create_node(value);
    if (node == NULL) {
        return 0;
    }

    current = &list->head;
    while (*current != NULL) {
        current = &(*current)->next;
    }

    *current = node;
    return 1;
}

static int append_list(List **lists, List *list)
{
    List **current;

    if (lists == NULL || list == NULL) {
        return 0;
    }

    current = lists;
    while (*current != NULL) {
        current = &(*current)->next;
    }

    *current = list;
    return 1;
}

static Node *reverse_list(Node *head)
{
    Node *previous = NULL;

    while (head != NULL) {
        Node *next = head->next;
        head->next = previous;
        previous = head;
        head = next;
    }

    return previous;
}

static void reverse_each_list(List *lists)
{
    while (lists != NULL) {
        lists->head = reverse_list(lists->head);
        lists = lists->next;
    }
}

static void print_lists(const List *lists)
{
    while (lists != NULL) {
        const Node *node = lists->head;

        putchar('[');
        while (node != NULL) {
            printf("%d", node->value);
            if (node->next != NULL) {
                printf(", ");
            }
            node = node->next;
        }
        puts("]");

        lists = lists->next;
    }
}

static void free_lists(List *lists)
{
    while (lists != NULL) {
        List *next_list = lists->next;
        Node *node = lists->head;

        while (node != NULL) {
            Node *next_node = node->next;
            free(node);
            node = next_node;
        }

        free(lists);
        lists = next_list;
    }
}

int main(void)
{
    static const int values[][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    List *lists = NULL;
    size_t i;
    size_t j;

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        List *list = create_list();

        if (list == NULL) {
            free_lists(lists);
            fputs("Memory allocation failed.\n", stderr);
            return EXIT_FAILURE;
        }

        if (!append_list(&lists, list)) {
            free(list);
            free_lists(lists);
            fputs("Failed to append list.\n", stderr);
            return EXIT_FAILURE;
        }

        for (j = 0; j < sizeof(values[i]) / sizeof(values[i][0]); ++j) {
            if (!append_value(list, values[i][j])) {
                free_lists(lists);
                fputs("Memory allocation failed.\n", stderr);
                return EXIT_FAILURE;
            }
        }
    }

    reverse_each_list(lists);
    print_lists(lists);
    free_lists(lists);

    return EXIT_SUCCESS;
}