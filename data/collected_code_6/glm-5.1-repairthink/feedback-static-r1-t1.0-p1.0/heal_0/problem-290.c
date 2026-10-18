#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    size_t length;
} List;

typedef struct ListOfLists {
    List **lists;
    size_t count;
    size_t capacity;
} ListOfLists;

void list_init(List *list) {
    list->head = NULL;
    list->length = 0;
}

void list_add(List *list, int value) {
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) {
        exit(EXIT_FAILURE);
    }
    int *data = malloc(sizeof(int));
    if (!data) {
        free(new_node);
        exit(EXIT_FAILURE);
    }
    *data = value;
    new_node->data = data;
    new_node->next = list->head;
    list->head = new_node;
    list->length++;
}

void list_free(List *list) {
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    list->head = NULL;
    list->length = 0;
}

void lol_init(ListOfLists *lol) {
    lol->lists = NULL;
    lol->count = 0;
    lol->capacity = 0;
}

void lol_add(ListOfLists *lol, List *list) {
    if (lol->count >= lol->capacity) {
        size_t new_capacity = lol->capacity == 0 ? 4 : lol->capacity * 2;
        List **new_lists = realloc(lol->lists, new_capacity * sizeof(List *));
        if (!new_lists) {
            exit(EXIT_FAILURE);
        }
        lol->lists = new_lists;
        lol->capacity = new_capacity;
    }
    lol->lists[lol->count++] = list;
}

void lol_free(ListOfLists *lol) {
    for (size_t i = 0; i < lol->count; i++) {
        list_free(lol->lists[i]);
        free(lol->lists[i]);
    }
    free(lol->lists);
    lol->lists = NULL;
    lol->count = 0;
    lol->capacity = 0;
}

ListOfLists find_lists_with_max_length(ListOfLists *input) {
    ListOfLists result;
    lol_init(&result);

    if (input->count == 0) {
        return result;
    }

    size_t max_len = 0;
    for (size_t i = 0; i < input->count; i++) {
        if (input->lists[i]->length > max_len) {
            max_len = input->lists[i]->length;
        }
    }

    for (size_t i = 0; i < input->count; i++) {
        if (input->lists[i]->length == max_len) {
            lol_add(&result, input->lists[i]);
        }
    }

    return result;
}

int main(void) {
    ListOfLists input;
    lol_init(&input);

    List *l1 = malloc(sizeof(List));
    if (!l1) exit(EXIT_FAILURE);
    list_init(l1);
    list_add(l1, 1);
    list_add(l1, 2);
    lol_add(&input, l1);

    List *l2 = malloc(sizeof(List));
    if (!l2) exit(EXIT_FAILURE);
    list_init(l2);
    list_add(l2, 3);
    list_add(l2, 4);
    list_add(l2, 5);
    lol_add(&input, l2);

    List *l3 = malloc(sizeof(List));
    if (!l3) exit(EXIT_FAILURE);
    list_init(l3);
    list_add(l3, 6);
    list_add(l3, 7);
    list_add(l3, 8);
    lol_add(&input, l3);

    ListOfLists max_lists = find_lists_with_max_length(&input);

    printf("Number of lists with max length: %zu\n", max_lists.count);
    for (size_t i = 0; i < max_lists.count; i++) {
        printf("List %zu length: %zu\n", i, max_lists.lists[i]->length);
    }

    max_lists.lists = NULL;
    max_lists.count = 0;
    max_lists.capacity = 0;
    lol_free(&input);

    return 0;
}