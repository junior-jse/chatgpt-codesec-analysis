#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

size_t count_lists(Node **array_of_lists, size_t num_lists) {
    size_t valid_count = 0;
    
    if (array_of_lists == NULL) {
        return 0;
    }

    for (size_t i = 0; i < num_lists; i++) {
        if (array_of_lists[i] != NULL) {
            valid_count++;
        }
    }

    return valid_count;
}

int main(void) {
    Node node1 = {NULL, NULL};
    Node node2 = {NULL, &node1};
    Node *list1 = &node2;
    Node *list2 = NULL;
    Node *list3 = &node1;

    Node **lists = malloc(3 * sizeof(Node *));
    if (lists == NULL) {
        return 1;
    }

    lists[0] = list1;
    lists[1] = list2;
    lists[2] = list3;

    size_t count = count_lists(lists, 3);
    printf("%zu\n", count);

    free(lists);
    lists = NULL;

    return 0;
}