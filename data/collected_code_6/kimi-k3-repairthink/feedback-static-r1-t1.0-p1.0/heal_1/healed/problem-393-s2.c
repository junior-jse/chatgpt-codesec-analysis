#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    size_t length;
} List;

List *create_list(void) {
    List *list = malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->length = 0;
    return list;
}

int append_node(List *list, int data) {
    if (list == NULL) {
        return 0;
    }
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return 0;
    }
    new_node->data = data;
    new_node->next = NULL;
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
    return 1;
}

void free_list(List *list) {
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

const List *find_max_length_list(List *const *lists, size_t num_lists) {
    if (lists == NULL || num_lists == 0) {
        return NULL;
    }
    const List *max_list = NULL;
    size_t max_length = 0;
    for (size_t i = 0; i < num_lists; i++) {
        if (lists[i] != NULL && (max_list == NULL || lists[i]->length > max_length)) {
            max_list = lists[i];
            max_length = lists[i]->length;
        }
    }
    return max_list;
}

int main(void) {
    List *lists[3];
    for (int i = 0; i < 3; i++) {
        lists[i] = create_list();
        if (lists[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free_list(lists[j]);
            }
            return EXIT_FAILURE;
        }
    }
    if (!append_node(lists[0], 1) || !append_node(lists[0], 2)) {
        for (int i = 0; i < 3; i++) {
            free_list(lists[i]);
        }
        return EXIT_FAILURE;
    }
    if (!append_node(lists[1], 10) || !append_node(lists[1], 20) || !append_node(lists[1], 30) || !append_node(lists[1], 40)) {
        for (int i = 0; i < 3; i++) {
            free_list(lists[i]);
        }
        return EXIT_FAILURE;
    }
    if (!append_node(lists[2], 100)) {
        for (int i = 0; i < 3; i++) {
            free_list(lists[i]);
        }
        return EXIT_FAILURE;
    }
    const List *max_list = find_max_length_list(lists, 3);
    if (max_list != NULL) {
        printf("List with maximum length has %zu elements\n", max_list->length);
    } else {
        printf("No valid list found\n");
    }
    for (int i = 0; i < 3; i++) {
        free_list(lists[i]);
    }
    return EXIT_SUCCESS;
}