#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    struct List *next;
} List;

List *create_list() {
    List *l = (List *)malloc(sizeof(List));
    if (l == NULL) {
        exit(EXIT_FAILURE);
    }
    l->head = NULL;
    l->next = NULL;
    return l;
}

Node *create_node(int data) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (n == NULL) {
        exit(EXIT_FAILURE);
    }
    n->data = data;
    n->next = NULL;
    return n;
}

int *get_last_elements(List *lists, int *count) {
    int size = 0;
    List *curr = lists;
    while (curr != NULL) {
        size++;
        curr = curr->next;
    }
    if (size == 0) {
        *count = 0;
        return NULL;
    }
    int *result = (int *)malloc(sizeof(int) * size);
    if (result == NULL) {
        exit(EXIT_FAILURE);
    }
    curr = lists;
    int i = 0;
    while (curr != NULL) {
        if (curr->head != NULL) {
            Node *n = curr->head;
            while (n->next != NULL) {
                n = n->next;
            }
            result[i] = n->data;
        } else {
            result[i] = 0;
        }
        i++;
        curr = curr->next;
    }
    *count = size;
    return result;
}

void free_lists(List *lists) {
    List *curr_l = lists;
    while (curr_l != NULL) {
        Node *curr_n = curr_l->head;
        while (curr_n != NULL) {
            Node *temp_n = curr_n;
            curr_n = curr_n->next;
            free(temp_n);
        }
        List *temp_l = curr_l;
        curr_l = curr_l->next;
        free(temp_l);
    }
}

int main() {
    List *l1 = create_list();
    l1->head = create_node(1);
    l1->head->next = create_node(2);
    l1->head->next->next = create_node(3);

    List *l2 = create_list();
    l2->head = create_node(4);
    l2->head->next = create_node(5);

    List *l3 = create_list();
    l3->head = create_node(6);

    l1->next = l2;
    l2->next = l3;

    int count = 0;
    int *last_elements = get_last_elements(l1, &count);

    for (int i = 0; i < count; i++) {
        printf("%d ", last_elements[i]);
    }
    printf("\n");

    free(last_elements);
    free_lists(l1);

    return 0;
}