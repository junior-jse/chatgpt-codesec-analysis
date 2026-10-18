#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Node *create_node(int value)
{
    struct Node *new_node = malloc(sizeof(*new_node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}

struct Node *append_node(struct Node *head, int value)
{
    struct Node *new_node = create_node(value);
    if (new_node == NULL) {
        return head;
    }
    if (head == NULL) {
        return new_node;
    }
    struct Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
    return head;
}

struct Node *add_lists(const struct Node *list1, const struct Node *list2)
{
    struct Node *result = NULL;
    struct Node *tail = NULL;
    int carry = 0;

    while (list1 != NULL || list2 != NULL || carry != 0) {
        int sum = carry;
        if (list1 != NULL) {
            sum += list1->data;
            list1 = list1->next;
        }
        if (list2 != NULL) {
            sum += list2->data;
            list2 = list2->next;
        }

        struct Node *new_node = create_node(sum % 10);
        if (new_node == NULL) {
            while (result != NULL) {
                struct Node *temp = result;
                result = result->next;
                free(temp);
            }
            return NULL;
        }

        carry = sum / 10;

        if (result == NULL) {
            result = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    return result;
}

void print_list(const struct Node *head)
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

void free_list(struct Node *head)
{
    while (head != NULL) {
        struct Node *temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void)
{
    struct Node *list1 = NULL;
    struct Node *list2 = NULL;

    list1 = append_node(list1, 2);
    list1 = append_node(list1, 4);
    list1 = append_node(list1, 3);

    list2 = append_node(list2, 5);
    list2 = append_node(list2, 6);
    list2 = append_node(list2, 4);

    if (list1 == NULL || list2 == NULL) {
        fprintf(stderr, "Failed to build input lists\n");
        free_list(list1);
        free_list(list2);
        return EXIT_FAILURE;
    }

    printf("List 1: ");
    print_list(list1);
    printf("List 2: ");
    print_list(list2);

    struct Node *sum = add_lists(list1, list2);
    if (sum == NULL) {
        fprintf(stderr, "Failed to compute sum\n");
        free_list(list1);
        free_list(list2);
        return EXIT_FAILURE;
    }

    printf("Sum:    ");
    print_list(sum);

    free_list(list1);
    free_list(list2);
    free_list(sum);

    return EXIT_SUCCESS;
}