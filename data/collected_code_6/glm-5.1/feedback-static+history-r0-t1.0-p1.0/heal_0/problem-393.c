#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *next;
} Node;

size_t list_length(Node *head) {
    size_t len = 0;
    while (head != NULL) {
        len++;
        head = head->next;
    }
    return len;
}

Node *find_max_length_list(Node **lists, size_t count) {
    if (lists == NULL || count == 0) {
        return NULL;
    }

    Node *max_list = lists[0];
    size_t max_len = list_length(lists[0]);

    for (size_t i = 1; i < count; i++) {
        size_t current_len = list_length(lists[i]);
        if (current_len > max_len) {
            max_len = current_len;
            max_list = lists[i];
        }
    }

    return max_list;
}

Node *create_node(int val) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (n == NULL) {
        exit(EXIT_FAILURE);
    }
    n->val = val;
    n->next = NULL;
    return n;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

int main(void) {
    Node *list1 = create_node(1);
    list1->next = create_node(2);

    Node *list2 = create_node(3);
    list2->next = create_node(4);
    list2->next->next = create_node(5);
    list2->next->next->next = create_node(6);

    Node *list3 = create_node(7);

    Node *lists[] = {list1, list2, list3};
    size_t count = sizeof(lists) / sizeof(lists[0]);

    Node *max_list = find_max_length_list(lists, count);

    if (max_list != NULL) {
        printf("List with max length starts at node with value: %d\n", max_list->val);
        printf("Its length is: %zu\n", list_length(max_list));
    }

    for (size_t i = 0; i < count; i++) {
        free_list(lists[i]);
    }

    return 0;
}