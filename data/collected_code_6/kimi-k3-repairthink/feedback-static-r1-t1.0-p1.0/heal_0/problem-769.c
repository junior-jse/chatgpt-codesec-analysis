#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

bool contains(Node *head, int value)
{
    Node *current = head;
    while (current != NULL) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

Node *list_difference(Node *list1, Node *list2)
{
    Node *result = NULL;
    Node *current = list1;
    while (current != NULL) {
        if (!contains(list2, current->data)) {
            if (append_node(&result, current->data) != 0) {
                while (result != NULL) {
                    Node *temp = result;
                    result = result->next;
                    free(temp);
                }
                return NULL;
            }
        }
        current = current->next;
    }
    return result;
}

void free_list(Node *head)
{
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

void print_list(Node *head)
{
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void)
{
    Node *list1 = NULL;
    Node *list2 = NULL;
    Node *diff = NULL;

    if (append_node(&list1, 1) != 0 ||
        append_node(&list1, 2) != 0 ||
        append_node(&list1, 3) != 0 ||
        append_node(&list1, 4) != 0 ||
        append_node(&list1, 5) != 0) {
        free_list(list1);
        return EXIT_FAILURE;
    }

    if (append_node(&list2, 2) != 0 ||
        append_node(&list2, 4) != 0) {
        free_list(list1);
        free_list(list2);
        return EXIT_FAILURE;
    }

    diff = list_difference(list1, list2);
    if (diff == NULL) {
        free_list(list1);
        free_list(list2);
        return EXIT_FAILURE;
    }

    printf("List 1: ");
    print_list(list1);
    printf("List 2: ");
    print_list(list2);
    printf("Difference: ");
    print_list(diff);

    free_list(list1);
    free_list(list2);
    free_list(diff);

    return EXIT_SUCCESS;
}