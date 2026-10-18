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

static int append_node(Node **head, int value)
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

static int reverse_each_list(List *lists)
{
    List *current;

    if (lists == NULL) {
        return 0;
    }

    for (current = lists; current != NULL; current = current->next) {
        current->head = reverse_list(current->head);
    }

    return 1;
}

static int print_lists(const List *lists)
{
    const List *list;

    if (lists == NULL) {
        return 0;
    }

    for (list = lists; list != NULL; list = list->next) {
        const Node *node;

        for (node = list->head; node != NULL; node = node->next) {
            if (printf("%d%s", node->value, node->next == NULL ? "" : " ") < 0) {
                return 0;
            }
        }

        if (putchar('\n') == EOF) {
            return 0;
        }
    }

    return 1;
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

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        List *list = create_list();
        size_t j;

        if (list == NULL) {
            free_lists(lists);
            return EXIT_FAILURE;
        }

        for (j = 0; j < sizeof(values[i]) / sizeof(values[i][0]); ++j) {
            if (!append_node(&list->head, values[i][j])) {
                free_lists(list);
                free_lists(lists);
                return EXIT_FAILURE;
            }
        }

        if (!append_list(&lists, list)) {
            free_lists(list);
            free_lists(lists);
            return EXIT_FAILURE;
        }
    }

    if (!reverse_each_list(lists) || !print_lists(lists)) {
        free_lists(lists);
        return EXIT_FAILURE;
    }

    free_lists(lists);
    return EXIT_SUCCESS;
}