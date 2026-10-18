#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Node *create_node(int data) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

struct Node *reverse_list(struct Node *head) {
    struct Node *prev = NULL;
    struct Node *curr = head;
    struct Node *next = NULL;

    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    return prev;
}

void reverse_all_lists(struct Node **lists, int count) {
    if (!lists || count <= 0) {
        return;
    }

    for (int i = 0; i < count; i++) {
        lists[i] = reverse_list(lists[i]);
    }
}

void print_list(struct Node *head) {
    struct Node *curr = head;
    while (curr) {
        printf("%d ", curr->data);
        curr = curr->next;
    }
    printf("\n");
}

void free_list(struct Node *head) {
    struct Node *curr = head;
    while (curr) {
        struct Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
}

struct Node *append_node(struct Node *head, int data) {
    struct Node *node = create_node(data);
    if (!head) {
        return node;
    }

    struct Node *curr = head;
    while (curr->next) {
        curr = curr->next;
    }
    curr->next = node;
    return head;
}

int main(void) {
    struct Node *lists[3] = {NULL, NULL, NULL};
    int list_count = 3;

    lists[0] = append_node(lists[0], 1);
    lists[0] = append_node(lists[0], 2);
    lists[0] = append_node(lists[0], 3);

    lists[1] = append_node(lists[1], 4);
    lists[1] = append_node(lists[1], 5);

    lists[2] = append_node(lists[2], 6);

    printf("Original lists:\n");
    for (int i = 0; i < list_count; i++) {
        print_list(lists[i]);
    }

    reverse_all_lists(lists, list_count);

    printf("Reversed lists:\n");
    for (int i = 0; i < list_count; i++) {
        print_list(lists[i]);
    }

    for (int i = 0; i < list_count; i++) {
        free_list(lists[i]);
    }

    return 0;
}