#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int *data;
    int size;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} ListOfLists;

void sort_by_index(ListOfLists *list, int index) {
    if (!list || !list->head || !list->head->next) return;

    Node *sorted = NULL;
    Node *current = list->head;

    while (current) {
        Node *next = current->next;
        if (!sorted || sorted->data[index] > current->data[index]) {
            current->next = sorted;
            sorted = current;
        } else {
            Node *temp = sorted;
            while (temp->next && temp->next->data[index] <= current->data[index]) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }

    list->head = sorted;
}

ListOfLists *create_list() {
    ListOfLists *list = (ListOfLists *)malloc(sizeof(ListOfLists));
    if (!list) {
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    return list;
}

Node *create_node(int *data, int size) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        exit(EXIT_FAILURE);
    }
    node->data = (int *)malloc(size * sizeof(int));
    if (!node->data) {
        free(node);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++) {
        node->data[i] = data[i];
    }
    node->size = size;
    node->next = NULL;
    return node;
}

void append_node(ListOfLists *list, Node *node) {
    if (!list->head) {
        list->head = node;
        return;
    }
    Node *temp = list->head;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = node;
}

void free_list(ListOfLists *list) {
    if (!list) return;
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}

void print_list(ListOfLists *list) {
    if (!list) return;
    Node *current = list->head;
    while (current) {
        printf("[");
        for (int i = 0; i < current->size; i++) {
            if (i > 0) printf(", ");
            printf("%d", current->data[i]);
        }
        printf("]");
        if (current->next) printf(" -> ");
        current = current->next;
    }
    printf("\n");
}

int main() {
    ListOfLists *list = create_list();

    int arr1[] = {3, 1, 4};
    int arr2[] = {1, 5, 9};
    int arr3[] = {2, 6, 5};
    int arr4[] = {0, 3, 8};

    append_node(list, create_node(arr1, 3));
    append_node(list, create_node(arr2, 3));
    append_node(list, create_node(arr3, 3));
    append_node(list, create_node(arr4, 3));

    printf("Before sorting:\n");
    print_list(list);

    sort_by_index(list, 0);

    printf("After sorting by index 0:\n");
    print_list(list);

    sort_by_index(list, 2);

    printf("After sorting by index 2:\n");
    print_list(list);

    free_list(list);
    return 0;
}