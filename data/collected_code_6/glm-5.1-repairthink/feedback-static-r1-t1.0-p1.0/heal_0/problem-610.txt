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

    if (k == 1) {
        struct Node *temp = *head;
        *head = (*head)->next;
        free(temp);
        return 0;
    }

    struct Node *current = *head;
    for (int i = 1; i < k - 1; ++i) {
        if (current->next == NULL) {
            return -1;
        }
        current = current->next;
    }

    if (current->next == NULL) {
        return -1;
    }

    struct Node *temp = current->next;
    current->next = temp->next;
    free(temp);

    return 0;
}

void insert(struct Node **head, int data) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = *head;
    *head = new_node;
}

void free_list(struct Node **head) {
    if (head == NULL) {
        return;
    }
    struct Node *current = *head;
    struct Node *next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    *head = NULL;
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

    insert(&head, 5);
    insert(&head, 4);
    insert(&head, 3);
    insert(&head, 2);
    insert(&head, 1);

    print_list(head);

    if (remove_kth(&head, 3) == 0) {
        print_list(head);
    }

    if (remove_kth(&head, 10) == -1) {
        printf("Index out of bounds\n");
    }

    free_list(&head);

    return 0;
}