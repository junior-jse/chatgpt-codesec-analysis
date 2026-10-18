#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct Sublist {
    Node *head;
    struct Sublist *next;
} Sublist;

static size_t sublist_length(const Node *head)
{
    size_t length = 0;

    while (head != NULL) {
        ++length;
        head = head->next;
    }

    return length;
}

static const Sublist *find_longest_sublist(const Sublist *lists)
{
    const Sublist *longest = NULL;
    size_t maximum_length = 0;

    while (lists != NULL) {
        const size_t length = sublist_length(lists->head);

        if (longest == NULL || length > maximum_length) {
            longest = lists;
            maximum_length = length;
        }

        lists = lists->next;
    }

    return longest;
}

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

static void free_nodes(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

static void free_sublists(Sublist *lists)
{
    while (lists != NULL) {
        Sublist *next = lists->next;
        free_nodes(lists->head);
        free(lists);
        lists = next;
    }
}

static int append_value(Node **head, int value)
{
    Node *node;
    Node **link;

    if (head == NULL) {
        return 0;
    }

    node = create_node(value);
    if (node == NULL) {
        return 0;
    }

    link = head;
    while (*link != NULL) {
        link = &(*link)->next;
    }

    *link = node;
    return 1;
}

static int append_sublist(Sublist **lists, const int *values, size_t count)
{
    Sublist *sublist;
    Sublist **link;
    size_t index;

    if (lists == NULL || (count > 0 && values == NULL)) {
        return 0;
    }

    sublist = malloc(sizeof(*sublist));
    if (sublist == NULL) {
        return 0;
    }

    sublist->head = NULL;
    sublist->next = NULL;

    for (index = 0; index < count; ++index) {
        if (!append_value(&sublist->head, values[index])) {
            free_nodes(sublist->head);
            free(sublist);
            return 0;
        }
    }

    link = lists;
    while (*link != NULL) {
        link = &(*link)->next;
    }

    *link = sublist;
    return 1;
}

static void print_sublist(const Node *head)
{
    while (head != NULL) {
        printf("%d", head->value);
        head = head->next;

        if (head != NULL) {
            printf(" ");
        }
    }

    printf("\n");
}

int main(void)
{
    const int first[] = {1, 2, 3};
    const int second[] = {4, 5, 6, 7, 8};
    const int third[] = {9, 10, 11, 12};
    Sublist *lists = NULL;
    const Sublist *longest;

    if (!append_sublist(&lists, first, sizeof(first) / sizeof(first[0])) ||
        !append_sublist(&lists, second, sizeof(second) / sizeof(second[0])) ||
        !append_sublist(&lists, third, sizeof(third) / sizeof(third[0]))) {
        fprintf(stderr, "Failed to allocate memory.\n");
        free_sublists(lists);
        return EXIT_FAILURE;
    }

    longest = find_longest_sublist(lists);
    if (longest == NULL) {
        fprintf(stderr, "No sublists found.\n");
        free_sublists(lists);
        return EXIT_FAILURE;
    }

    print_sublist(longest->head);
    free_sublists(lists);
    return EXIT_SUCCESS;
}