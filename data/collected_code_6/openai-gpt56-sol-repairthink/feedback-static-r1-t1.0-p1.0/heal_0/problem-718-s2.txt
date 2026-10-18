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

    node = create_node(value);
    if (node == NULL) {
        return 0;
    }

    if (*tail == NULL) {
        *head = node;
    } else {
        (*tail)->next = node;
    }

    *tail = node;
    return 1;
}

static Node *create_alternate_list(const Node *source)
{
    Node *result = NULL;
    Node *tail = NULL;
    int take = 1;

    while (source != NULL) {
        if (take != 0 && !append_node(&result, &tail, source->value)) {
            free_list(result);
            return NULL;
        }

        take = !take;
        source = source->next;
    }

    return result;
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

    putchar('\n');
}

int main(void)
{
    const int values[] = {1, 2, 3, 4, 5, 6, 7};
    const size_t count = sizeof(values) / sizeof(values[0]);
    Node *source = NULL;
    Node *source_tail = NULL;
    Node *alternate;
    size_t i;

    for (i = 0; i < count; ++i) {
        if (!append_node(&source, &source_tail, values[i])) {
            fprintf(stderr, "Failed to allocate source list\n");
            free_list(source);
            return EXIT_FAILURE;
        }
    }

    alternate = create_alternate_list(source);
    if (alternate == NULL && source != NULL) {
        fprintf(stderr, "Failed to allocate alternate list\n");
        free_list(source);
        return EXIT_FAILURE;
    }

    print_list(source);
    print_list(alternate);

    free_list(alternate);
    free_list(source);
    return EXIT_SUCCESS;
}