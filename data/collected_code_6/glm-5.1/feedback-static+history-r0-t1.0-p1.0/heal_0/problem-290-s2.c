#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct ListOfLists {
    Node *list;
    int length;
    struct ListOfLists *next;
} ListOfLists;

typedef struct ResultList {
    ListOfLists **lists;
    int count;
} ResultList;

ResultList find_max_length_lists(ListOfLists *head) {
    ResultList result;
    result.lists = NULL;
    result.count = 0;

    if (head == NULL) {
        return result;
    }

    int max_len = 0;
    ListOfLists *curr = head;

    while (curr != NULL) {
        if (curr->length > max_len) {
            max_len = curr->length;
        }
        curr = curr->next;
    }

    int capacity = 10;
    result.lists = (ListOfLists **)malloc(capacity * sizeof(ListOfLists *));
    if (result.lists == NULL) {
        result.count = 0;
        return result;
    }

    curr = head;
    while (curr != NULL) {
        if (curr->length == max_len) {
            if (result.count >= capacity) {
                capacity *= 2;
                ListOfLists **tmp = (ListOfLists **)realloc(result.lists, capacity * sizeof(ListOfLists *));
                if (tmp == NULL) {
                    free(result.lists);
                    result.lists = NULL;
                    result.count = 0;
                    return result;
                }
                result.lists = tmp;
            }
            result.lists[result.count++] = curr;
        }
        curr = curr->next;
    }

    return result;
}

ListOfLists *create_list_of_lists(int lengths[], int size) {
    ListOfLists *head = NULL;
    ListOfLists *tail = NULL;

    for (int i = 0; i < size; i++) {
        ListOfLists *node = (ListOfLists *)malloc(sizeof(ListOfLists));
        node->length = lengths[i];
        node->list = NULL;
        node->next = NULL;

        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    return head;
}

void free_list_of_lists(ListOfLists *head) {
    ListOfLists *curr = head;
    while (curr != NULL) {
        ListOfLists *next = curr->next;
        free(curr);
        curr = next;
    }
}

int main() {
    int lengths[] = {3, 5, 2, 5, 4, 5};
    int size = sizeof(lengths) / sizeof(lengths[0]);

    ListOfLists *lol = create_list_of_lists(lengths, size);
    ResultList result = find_max_length_lists(lol);

    printf("Found %d list(s) with max length:\n", result.count);
    for (int i = 0; i < result.count; i++) {
        printf("List at address %p with length %d\n", (void *)result.lists[i], result.lists[i]->length);
    }

    free(result.lists);
    free_list_of_lists(lol);

    return 0;
}