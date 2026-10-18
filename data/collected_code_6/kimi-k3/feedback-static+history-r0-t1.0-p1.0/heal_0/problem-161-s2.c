#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data)
{
    Node *new_node = malloc(sizeof(Node));
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

int contains_value(const Node *head, int data)
{
    while (head != NULL) {
        if (head->data == data) {
            return 1;
        }
        head = head->next;
    }
    return 0;
}

void remove_elements(Node **list, const Node *remove_list)
{
    Node *current = *list;
    Node *prev = NULL;

    while (current != NULL) {
        if (contains_value(remove_list, current->data)) {
            Node *temp = current;
            if (prev == NULL) {
                *list = current->next;
            } else {
                prev->next = current->next;
            }
            current = current->next;
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void print_list(const Node *head)
{
    while (head != NULL) {
        printf("%d", head->data);
        if (head->next != NULL) {
            printf(" -> ");
        }
        head = head->next;
    }
    printf("\n");
}

void free_list(Node *head)
{
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void)
{
    Node *list = NULL;
    Node *remove_list = NULL;

    if (append_node(&list, 1) != 0 ||
        append_node(&list, 2) != 0 ||
        append_node(&list, 3) != 0 ||
        append_node(&list, 4) != 0 ||
        append_node(&list, 5) != 0) {
        fprintf(stderr, "Memory allocation failed\n");
        free_list(list);
        return EXIT_FAILURE;
    }

    if (append_node(&remove_list, 2) != 0 ||
        append_node(&remove_list, 4) != 0) {
        fprintf(stderr, "Memory allocation failed\n");
        free_list(list);
        free_list(remove_list);
        return EXIT_FAILURE;
    }

    printf("Original list: ");
    print_list(list);

    printf("Elements to remove: ");
    print_list(remove_list);

    remove_elements(&list, remove_list);

    printf("Resulting list: ");
    print_list(list);

    free_list(list);
    free_list(remove_list);

    return EXIT_SUCCESS;
}