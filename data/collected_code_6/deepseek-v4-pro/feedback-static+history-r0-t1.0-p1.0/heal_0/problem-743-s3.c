#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    int length;
} List;

Node *create_node(int data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

List *create_list() {
    List *list = (List *)malloc(sizeof(List));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    list->length = 0;
    return list;
}

void append(List *list, int data) {
    Node *new_node = create_node(data);
    if (!new_node) {
        return;
    }
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->length++;
}

void rotate_right(List *list, int k) {
    if (list == NULL || list->head == NULL || list->length <= 1 || k <= 0) {
        return;
    }

    k = k % list->length;
    if (k == 0) {
        return;
    }

    Node *current = list->head;
    int steps = list->length - k - 1;

    for (int i = 0; i < steps; i++) {
        current = current->next;
    }

    Node *new_head = current->next;
    current->next = NULL;

    Node *tail = new_head;
    while (tail->next != NULL) {
        tail = tail->next;
    }
    tail->next = list->head;
    list->head = new_head;
}

void free_list(List *list) {
    if (list == NULL) {
        return;
    }
    Node *current = list->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

void print_list(List *list) {
    if (list == NULL || list->head == NULL) {
        printf("Empty list\n");
        return;
    }
    Node *current = list->head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

int main() {
    List *list = create_list();
    if (!list) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    append(list, 1);
    append(list, 2);
    append(list, 3);
    append(list, 4);
    append(list, 5);

    printf("Original list: ");
    print_list(list);

    rotate_right(list, 2);
    printf("After rotating by 2: ");
    print_list(list);

    rotate_right(list, 7);
    printf("After rotating by 7: ");
    print_list(list);

    rotate_right(list, 0);
    printf("After rotating by 0: ");
    print_list(list);

    free_list(list);
    return EXIT_SUCCESS;
}