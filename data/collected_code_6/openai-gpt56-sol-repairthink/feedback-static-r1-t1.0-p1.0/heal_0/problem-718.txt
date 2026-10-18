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

static Node *create_alternate_list(const Node *source, int *success)
{
    Node *head = NULL;
    Node *tail = NULL;
    int take = 1;

    if (success == NULL) {
        return NULL;
    }

    *success = 0;

    while (source != NULL) {
        if (take && !append_node(&head, &tail, source->value)) {
            free_list(head);
            return NULL;
        }

        take = !take;
        source = source->next;
    }

    *success = 1;
    return head;
}

static void print_list(const Node *head)
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
    Node *source = NULL;
    Node *source_tail = NULL;
    Node *alternate = NULL;
    int count;
    int success;

    if (scanf("%d", &count) != 1 || count < 0) {
        fprintf(stderr, "Invalid list size\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < count; ++i) {
        int value;

        if (scanf("%d", &value) != 1) {
            fprintf(stderr, "Invalid list element\n");
            free_list(source);
            return EXIT_FAILURE;
        }

        if (!append_node(&source, &source_tail, value)) {
            fprintf(stderr, "Memory allocation failed\n");
            free_list(source);
            return EXIT_FAILURE;
        }
    }

    alternate = create_alternate_list(source, &success);
    if (!success) {
        fprintf(stderr, "Memory allocation failed\n");
        free_list(source);
        return EXIT_FAILURE;
    }

    print_list(alternate);

    free_list(alternate);
    free_list(source);
    return EXIT_SUCCESS;
}