#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int data;
    struct ListNode *next;
} ListNode;

typedef struct {
    ListNode *head;
    size_t length;
} List;

List *create_list(void)
{
    List *list = malloc(sizeof(List));
    if (list != NULL) {
        list->head = NULL;
        list->length = 0;
    }
    return list;
}

int append_to_list(List *list, int value)
{
    if (list == NULL) {
        return -1;
    }

    ListNode *new_node = malloc(sizeof(ListNode));
    if (new_node == NULL) {
        return -1;
    }

    new_node->data = value;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        ListNode *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }

    list->length++;
    return 0;
}

void free_list(List *list)
{
    if (list == NULL) {
        return;
    }

    ListNode *current = list->head;
    ListNode *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(list);
}

List *find_max_length_list(List **lists, size_t num_lists)
{
    if (lists == NULL || num_lists == 0) {
        return NULL;
    }

    List *max_list = NULL;
    size_t max_length = 0;

    for (size_t i = 0; i < num_lists; i++) {
        if (lists[i] != NULL && (max_list == NULL || lists[i]->length > max_length)) {
            max_list = lists[i];
            max_length = lists[i]->length;
        }
    }

    return max_list;
}

int main(void)
{
    List *lists[3] = {NULL, NULL, NULL};
    List *result = NULL;
    int status = 0;

    for (int i = 0; i < 3; i++) {
        lists[i] = create_list();
        if (lists[i] == NULL) {
            status = -1;
            goto cleanup;
        }
    }

    if (append_to_list(lists[0], 1) != 0 ||
        append_to_list(lists[0], 2) != 0) {
        status = -1;
        goto cleanup;
    }

    if (append_to_list(lists[1], 10) != 0 ||
        append_to_list(lists[1], 20) != 0 ||
        append_to_list(lists[1], 30) != 0 ||
        append_to_list(lists[1], 40) != 0) {
        status = -1;
        goto cleanup;
    }

    if (append_to_list(lists[2], 100) != 0) {
        status = -1;
        goto cleanup;
    }

    result = find_max_length_list(lists, 3);

    if (result != NULL) {
        printf("List with maximum length: %zu elements\n", result->length);
    } else {
        printf("No valid lists found\n");
    }

cleanup:
    for (int i = 0; i < 3; i++) {
        if (lists[i] != NULL) {
            free_list(lists[i]);
        }
    }

    return status == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}