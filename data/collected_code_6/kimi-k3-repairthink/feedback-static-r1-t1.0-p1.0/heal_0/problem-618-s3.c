#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int list_length(Node *head) {
    int length = 0;
    Node *current = head;
    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;
}

int divide_lists(Node *list1, Node *list2, Node **result) {
    if (result == NULL) {
        return -1;
    }
    *result = NULL;

    int len1 = list_length(list1);
    int len2 = list_length(list2);

    if (len1 != len2) {
        return -1;
    }

    Node *current1 = list1;
    Node *current2 = list2;
    Node *result_tail = NULL;

    while (current1 != NULL && current2 != NULL) {
        if (current2->data == 0) {
            free_list(*result);
            *result = NULL;
            return -1;
        }

        Node *new_node = create_node(current1->data / current2->data);
        if (new_node == NULL) {
            free_list(*result);
            *result = NULL;
            return -1;
        }

        if (*result == NULL) {
            *result = new_node;
            result_tail = new_node;
        } else {
            result_tail->next = new_node;
            result_tail = new_node;
        }

        current1 = current1->next;
        current2 = current2->next;
    }

    return 0;
}

int main(void) {
    Node *list1 = NULL;
    Node *list2 = NULL;
    Node *result = NULL;

    list1 = create_node(10);
    if (list1 == NULL) {
        return EXIT_FAILURE;
    }
    list1->next = create_node(20);
    if (list1->next == NULL) {
        free_list(list1);
        return EXIT_FAILURE;
    }
    list1->next->next = create_node(30);
    if (list1->next->next == NULL) {
        free_list(list1);
        return EXIT_FAILURE;
    }

    list2 = create_node(2);
    if (list2 == NULL) {
        free_list(list1);
        return EXIT_FAILURE;
    }
    list2->next = create_node(5);
    if (list2->next == NULL) {
        free_list(list1);
        free_list(list2);
        return EXIT_FAILURE;
    }
    list2->next->next = create_node(3);
    if (list2->next->next == NULL) {
        free_list(list1);
        free_list(list2);
        return EXIT_FAILURE;
    }

    if (divide_lists(list1, list2, &result) != 0) {
        free_list(list1);
        free_list(list2);
        return EXIT_FAILURE;
    }

    Node *current = result;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");

    free_list(list1);
    free_list(list2);
    free_list(result);

    return EXIT_SUCCESS;
}