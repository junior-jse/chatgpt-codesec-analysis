#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *next;
} Node;

size_t list_length(Node *head) {
    size_t len = 0;
    Node *curr = head;
    while (curr != NULL) {
        len++;
        curr = curr->next;
    }
    return len;
}

Node *find_max_length_list(Node **lists, size_t count) {
    if (lists == NULL || count == 0) {
        return NULL;
    }

    Node *max_list = NULL;
    size_t max_len = 0;

    for (size_t i = 0; i < count; i++) {
        size_t curr_len = list_length(lists[i]);
        if (curr_len > max_len) {
            max_len = curr_len;
            max_list = lists[i];
        }
    }

    return max_list;
}

Node *create_node(int val) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (n == NULL) {
        return NULL;
    }
    n->val = val;
    n->next = NULL;
    return n;
}

void free_list(Node *head) {
    Node *curr = head;
    while (curr != NULL) {
        Node *next = curr->next;
        free(curr);
        curr = next;
    }
}

int main(void) {
    Node *list1 = create_node(1);
    Node *list2 = create_node(2);
    Node *list3 = create_node(3);

    if (list1 == NULL || list2 == NULL || list3 == NULL) {
        free(list1);
        free(list2);
        free(list3);
        return 1;
    }

    list1->next = create_node(4);
    if (list1->next == NULL) {
        free_list(list1);
        free(list2);
        free(list3);
        return 1;
    }

    list2->next = create_node(5);
    if (list2->next == NULL) {
        free_list(list1);
        free_list(list2);
        free(list3);
        return 1;
    }

    list2->next->next = create_node(6);
    if (list2->next->next == NULL) {
        free_list(list1);
        free_list(list2);
        free(list3);
        return 1;
    }

    Node *lists[] = {list1, list2, list3};
    size_t count = sizeof(lists) / sizeof(lists[0]);

    Node *max_list = find_max_length_list(lists, count);

    if (max_list != NULL) {
        printf("List with max length starts at value: %d\n", max_list->val);
    } else {
        printf("All lists are empty or invalid input provided.\n");
    }

    free_list(list1);
    free_list(list2);
    free_list(list3);

    return 0;
}