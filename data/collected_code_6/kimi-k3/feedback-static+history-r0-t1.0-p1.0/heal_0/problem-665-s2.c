#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

int append_node(Node **head, int data)
{
    Node *new_node = create_node(data);
    if (new_node == NULL) {
        return -1;
    }
    if (*head == NULL) {
        *head = new_node;
        return 0;
    }
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return 0;
}

int shift_first_to_end(Node **head)
{
    if (head == NULL || *head == NULL || (*head)->next == NULL) {
        return -1;
    }
    Node *first = *head;
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    *head = first->next;
    current->next = first;
    first->next = NULL;
    return 0;
}

void print_list(const Node *head)
{
    const Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void free_list(Node **head)
{
    if (head == NULL) {
        return;
    }
    Node *current = *head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

int main(void)
{
    Node *head = NULL;
    int values[] = {1, 2, 3, 4, 5};
    size_t count = sizeof(values) / sizeof(values[0]);
    size_t i;

    for (i = 0; i < count; i++) {
        if (append_node(&head, values[i]) != 0) {
            fprintf(stderr, "Memory allocation failed\n");
            free_list(&head);
            return EXIT_FAILURE;
        }
    }

    printf("Original list: ");
    print_list(head);

    if (shift_first_to_end(&head) != 0) {
        fprintf(stderr, "Shift operation failed\n");
        free_list(&head);
        return EXIT_FAILURE;
    }

    printf("List after shifting first element to end: ");
    print_list(head);

    free_list(&head);
    return EXIT_SUCCESS;
}