#include <stdio.h>
#include <stdlib.h>

typedef struct Tuple {
    int first;
    int second;
} Tuple;

typedef struct Node {
    Tuple data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    Node *tail;
    size_t size;
} List;

List *list_create(void) {
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void list_destroy(List *list) {
    if (list == NULL) {
        return;
    }
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int list_append(List *list, Tuple tuple) {
    if (list == NULL) {
        return -1;
    }
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return -1;
    }
    new_node->data = tuple;
    new_node->next = NULL;

    if (list->tail == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
    return 0;
}

void list_print(const List *list) {
    if (list == NULL) {
        return;
    }
    const Node *current = list->head;
    while (current != NULL) {
        printf("(%d, %d) ", current->data.first, current->data.second);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    List *list = list_create();
    if (list == NULL) {
        fprintf(stderr, "Failed to create list\n");
        return 1;
    }

    Tuple t1 = {1, 2};
    Tuple t2 = {3, 4};
    Tuple t3 = {5, 6};

    if (list_append(list, t1) != 0) {
        fprintf(stderr, "Failed to append tuple\n");
        list_destroy(list);
        return 1;
    }
    if (list_append(list, t2) != 0) {
        fprintf(stderr, "Failed to append tuple\n");
        list_destroy(list);
        return 1;
    }
    if (list_append(list, t3) != 0) {
        fprintf(stderr, "Failed to append tuple\n");
        list_destroy(list);
        return 1;
    }

    printf("List contents: ");
    list_print(list);

    list_destroy(list);
    return 0;
}