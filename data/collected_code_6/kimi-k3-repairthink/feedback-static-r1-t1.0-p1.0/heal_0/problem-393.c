#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    size_t length;
} List;

int list_init(List *list) {
    if (list == NULL) {
        return -1;
    }
    list->head = NULL;
    list->length = 0;
    return 0;
}

int list_append(List *list, int data) {
    Node *new_node;
    Node *current;

    if (list == NULL) {
        return -1;
    }

    new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return -1;
    }

    new_node->data = data;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    list->length++;
    return 0;
}

void list_destroy(List *list) {
    Node *current;
    Node *next;

    if (list == NULL) {
        return;
    }

    current = list->head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->length = 0;
}

const List *find_max_length_list(const List *lists, size_t num_lists) {
    const List *max_list;
    size_t i;

    if (lists == NULL || num_lists == 0) {
        return NULL;
    }

    max_list = &lists[0];
    for (i = 1; i < num_lists; i++) {
        if (lists[i].length > max_list->length) {
            max_list = &lists[i];
        }
    }
    return max_list;
}

int main(void) {
    List lists[3];
    const List *max_list;
    size_t i;
    int result = 0;

    for (i = 0; i < 3; i++) {
        if (list_init(&lists[i]) != 0) {
            size_t j;
            for (j = 0; j < i; j++) {
                list_destroy(&lists[j]);
            }
            return EXIT_FAILURE;
        }
    }

    if (list_append(&lists[0], 1) != 0 ||
        list_append(&lists[0], 2) != 0) {
        result = EXIT_FAILURE;
        goto cleanup;
    }

    if (list_append(&lists[1], 10) != 0 ||
        list_append(&lists[1], 20) != 0 ||
        list_append(&lists[1], 30) != 0 ||
        list_append(&lists[1], 40) != 0) {
        result = EXIT_FAILURE;
        goto cleanup;
    }

    if (list_append(&lists[2], 100) != 0) {
        result = EXIT_FAILURE;
        goto cleanup;
    }

    max_list = find_max_length_list(lists, 3);
    if (max_list == NULL) {
        result = EXIT_FAILURE;
        goto cleanup;
    }

    printf("List with maximum length has %zu elements\n", max_list->length);

cleanup:
    for (i = 0; i < 3; i++) {
        list_destroy(&lists[i]);
    }

    return result;
}