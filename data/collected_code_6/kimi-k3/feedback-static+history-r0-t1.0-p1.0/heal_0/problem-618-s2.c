#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data)
{
    Node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

int append(Node **head, int data)
{
    Node *node = create_node(data);
    if (node == NULL) {
        return -1;
    }
    if (*head == NULL) {
        *head = node;
        return 0;
    }
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = node;
    return 0;
}

void free_list(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

Node *divide_lists(const Node *a, const Node *b, int *status)
{
    Node *result = NULL;
    Node *tail = NULL;
    *status = 0;

    while (a != NULL && b != NULL) {
        if (b->data == 0) {
            *status = -1;
            free_list(result);
            return NULL;
        }
        Node *node = create_node(a->data / b->data);
        if (node == NULL) {
            *status = -2;
            free_list(result);
            return NULL;
        }
        if (result == NULL) {
            result = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        a = a->next;
        b = b->next;
    }

    if (a != NULL || b != NULL) {
        *status = -3;
        free_list(result);
        return NULL;
    }

    return result;
}

void print_list(const Node *head)
{
    while (head != NULL) {
        printf("%d", head->data);
        if (head->next != NULL) {
            printf(" ");
        }
        head = head->next;
    }
    printf("\n");
}

int main(void)
{
    Node *list_a = NULL;
    Node *list_b = NULL;
    int status = 0;

    if (append(&list_a, 10) != 0 || append(&list_a, 20) != 0 ||
        append(&list_a, 30) != 0 || append(&list_a, 40) != 0) {
        free_list(list_a);
        return EXIT_FAILURE;
    }

    if (append(&list_b, 2) != 0 || append(&list_b, 4) != 0 ||
        append(&list_b, 5) != 0 || append(&list_b, 8) != 0) {
        free_list(list_a);
        free_list(list_b);
        return EXIT_FAILURE;
    }

    Node *result = divide_lists(list_a, list_b, &status);
    if (result == NULL) {
        fprintf(stderr, "Error: division failed (status %d)\n", status);
        free_list(list_a);
        free_list(list_b);
        return EXIT_FAILURE;
    }

    print_list(result);

    free_list(list_a);
    free_list(list_b);
    free_list(result);

    return EXIT_SUCCESS;
}