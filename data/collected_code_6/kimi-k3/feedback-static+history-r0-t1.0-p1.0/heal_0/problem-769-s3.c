#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

static Node *create_node(int data)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

static void append_node(Node **head, Node **tail, int data)
{
    Node *new_node = create_node(data);
    if (new_node == NULL) {
        return;
    }
    if (*head == NULL) {
        *head = new_node;
        *tail = new_node;
    } else {
        (*tail)->next = new_node;
        *tail = new_node;
    }
}

static int list_contains(const Node *head, int value)
{
    const Node *current = head;
    while (current != NULL) {
        if (current->data == value) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

Node *list_difference(const Node *list1, const Node *list2)
{
    Node *result_head = NULL;
    Node *result_tail = NULL;
    const Node *current = list1;

    while (current != NULL) {
        if (!list_contains(list2, current->data)) {
            Node *new_node = create_node(current->data);
            if (new_node == NULL) {
                while (result_head != NULL) {
                    Node *temp = result_head;
                    result_head = result_head->next;
                    free(temp);
                }
                return NULL;
            }
            if (result_head == NULL) {
                result_head = new_node;
                result_tail = new_node;
            } else {
                result_tail->next = new_node;
                result_tail = new_node;
            }
        }
        current = current->next;
    }
    return result_head;
}

void free_list(Node *head)
{
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

static void print_list(const Node *head)
{
    const Node *current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

int main(void)
{
    Node *list1 = NULL;
    Node *list1_tail = NULL;
    Node *list2 = NULL;
    Node *list2_tail = NULL;
    Node *diff = NULL;

    append_node(&list1, &list1_tail, 1);
    append_node(&list1, &list1_tail, 2);
    append_node(&list1, &list1_tail, 3);
    append_node(&list1, &list1_tail, 4);
    append_node(&list1, &list1_tail, 5);

    append_node(&list2, &list2_tail, 2);
    append_node(&list2, &list2_tail, 4);
    append_node(&list2, &list2_tail, 6);

    diff = list_difference(list1, list2);
    if (diff == NULL && list1 != NULL) {
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