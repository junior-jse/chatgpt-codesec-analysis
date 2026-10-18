#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data)
{
    Node *new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

Node *create_alternate_list(const Node *head)
{
    Node *new_head = NULL;
    Node *new_tail = NULL;
    const Node *current = head;
    int take = 1;

    while (current != NULL) {
        if (take) {
            Node *new_node = create_node(current->data);
            if (new_node == NULL) {
                while (new_head != NULL) {
                    Node *temp = new_head;
                    new_head = new_head->next;
                    free(temp);
                }
                return NULL;
            }
            if (new_tail == NULL) {
                new_head = new_node;
            } else {
                new_tail->next = new_node;
            }
            new_tail = new_node;
        }
        take = !take;
        current = current->next;
    }
    return new_head;
}

void free_list(Node *head)
{
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

void print_list(const Node *head)
{
    const Node *current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" ");
        }
        current = current->next;
    }
    printf("\n");
}

int main(void)
{
    Node *head = NULL;
    Node *tail = NULL;
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t count = sizeof(values) / sizeof(values[0]);
    size_t i;

    for (i = 0; i < count; ++i) {
        Node *new_node = create_node(values[i]);
        if (new_node == NULL) {
            free_list(head);
            return EXIT_FAILURE;
        }
        if (tail == NULL) {
            head = new_node;
        } else {
            tail->next = new_node;
        }
        tail = new_node;
    }

    printf("Original list: ");
    print_list(head);

    Node *alternate = create_alternate_list(head);
    if (alternate == NULL && head != NULL) {
        free_list(head);
        return EXIT_FAILURE;
    }

    printf("Alternate list: ");
    print_list(alternate);

    free_list(head);
    free_list(alternate);

    return EXIT_SUCCESS;
}