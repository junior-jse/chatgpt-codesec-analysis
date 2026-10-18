#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Node *create_node(int data) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void append_node(struct Node **head, int data) {
    struct Node *new_node = create_node(data);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    struct Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

void free_list(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *next = current->next;
        free(current);
        current = next;
    }
}

struct Node *rotate_left(struct Node *head, int k) {
    if (head == NULL || head->next == NULL || k <= 0) {
        return head;
    }

    struct Node *current = head;
    int length = 1;
    while (current->next != NULL) {
        current = current->next;
        length++;
    }

    k = k % length;
    if (k == 0) {
        return head;
    }

    current->next = head;
    current = head;
    for (int i = 1; i < k; i++) {
        current = current->next;
    }

    head = current->next;
    current->next = NULL;
    return head;
}

void print_list(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    struct Node *head = NULL;
    int arr[] = {1, 2, 3, 4, 5};
    int n = sizeof(arr) / sizeof(arr[0]);

    for (int i = 0; i < n; i++) {
        append_node(&head, arr[i]);
    }

    printf("Original list: ");
    print_list(head);

    int k = 2;
    head = rotate_left(head, k);

    printf("Rotated list by %d: ", k);
    print_list(head);

    free_list(head);
    return 0;
}