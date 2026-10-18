#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

int remove_kth(struct Node **head, int k) {
    if (head == NULL || *head == NULL || k < 1) {
        return -1;
    }

    struct Node *temp = *head;

    if (k == 1) {
        *head = temp->next;
        free(temp);
        return 0;
    }

    struct Node *prev = NULL;
    for (int i = 1; i < k && temp != NULL; i++) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        return -1;
    }

    prev->next = temp->next;
    free(temp);

    return 0;
}

void insert(struct Node **head, int data) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        return;
    }
    new_node->data = data;
    new_node->next = *head;
    *head = new_node;
}

void print_list(struct Node *head) {
    struct Node *temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void free_list(struct Node **head) {
    if (head == NULL) {
        return;
    }
    struct Node *current = *head;
    struct Node *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

int main(void) {
    struct Node *head = NULL;

    insert(&head, 5);
    insert(&head, 4);
    insert(&head, 3);
    insert(&head, 2);
    insert(&head, 1);

    print_list(head);

    remove_kth(&head, 3);

    print_list(head);

    remove_kth(&head, 1);

    print_list(head);

    remove_kth(&head, 10);

    free_list(&head);

    return 0;
}