#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
} List;

int count_lists(List **lists, int num_lists) {
    if (lists == NULL || num_lists <= 0) {
        return 0;
    }
    return num_lists;
}

Node *create_node(int data) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int main(void) {
    int num_lists = 3;
    List **lists = malloc((size_t)num_lists * sizeof(List *));
    if (lists == NULL) {
        return 1;
    }

    for (int i = 0; i < num_lists; i++) {
        lists[i] = malloc(sizeof(List));
        if (lists[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(lists[j]);
            }
            free(lists);
            return 1;
        }
        lists[i]->head = create_node(i + 1);
        if (lists[i]->head == NULL) {
            for (int j = 0; j <= i; j++) {
                free(lists[j]);
            }
            free(lists);
            return 1;
        }
    }

    int result = count_lists(lists, num_lists);
    printf("Number of lists: %d\n", result);

    for (int i = 0; i < num_lists; i++) {
        free_list(lists[i]->head);
        free(lists[i]);
    }
    free(lists);

    return 0;
}