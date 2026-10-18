#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    int length;
} List;

typedef struct ListOfLists {
    List **lists;
    int count;
    int capacity;
} ListOfLists;

ListOfLists *find_max_length_lists(ListOfLists *input) {
    if (input == NULL || input->count == 0) {
        return NULL;
    }

    int max_len = -1;
    for (int i = 0; i < input->count; i++) {
        if (input->lists[i] != NULL && input->lists[i]->length > max_len) {
            max_len = input->lists[i]->length;
        }
    }

    if (max_len == -1) {
        return NULL;
    }

    ListOfLists *result = (ListOfLists *)malloc(sizeof(ListOfLists));
    if (result == NULL) {
        return NULL;
    }

    result->capacity = input->count > 0 ? input->count : 1;
    result->lists = (List **)malloc(result->capacity * sizeof(List *));
    if (result->lists == NULL) {
        free(result);
        return NULL;
    }
    result->count = 0;

    for (int i = 0; i < input->count; i++) {
        if (input->lists[i] != NULL && input->lists[i]->length == max_len) {
            result->lists[result->count++] = input->lists[i];
        }
    }

    return result;
}

List *create_list(int length) {
    List *l = (List *)malloc(sizeof(List));
    if (l == NULL) {
        return NULL;
    }
    l->head = NULL;
    l->length = length;
    return l;
}

void free_list(List *l) {
    if (l == NULL) {
        return;
    }
    Node *current = l->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(l);
}

void free_list_of_lists(ListOfLists *lol) {
    if (lol == NULL) {
        return;
    }
    free(lol->lists);
    free(lol);
}

int main() {
    ListOfLists input;
    input.capacity = 4;
    input.count = 4;
    input.lists = (List **)malloc(input.capacity * sizeof(List *));
    if (input.lists == NULL) {
        return 1;
    }

    input.lists[0] = create_list(3);
    input.lists[1] = create_list(5);
    input.lists[2] = create_list(5);
    input.lists[3] = create_list(2);

    if (input.lists[0] == NULL || input.lists[1] == NULL || input.lists[2] == NULL || input.lists[3] == NULL) {
        for (int i = 0; i < 4; i++) {
            free_list(input.lists[i]);
        }
        free(input.lists);
        return 1;
    }

    ListOfLists *max_lists = find_max_length_lists(&input);

    if (max_lists != NULL) {
        printf("Max length lists count: %d\n", max_lists->count);
        for (int i = 0; i < max_lists->count; i++) {
            printf("List %d length: %d\n", i, max_lists->lists[i]->length);
        }
        free_list_of_lists(max_lists);
    }

    for (int i = 0; i < input.count; i++) {
        free_list(input.lists[i]);
    }
    free(input.lists);

    return 0;
}