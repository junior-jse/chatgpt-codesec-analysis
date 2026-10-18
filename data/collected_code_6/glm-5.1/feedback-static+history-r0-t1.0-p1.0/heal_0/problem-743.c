#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* create_node(int data) {
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_list(struct Node* head) {
    struct Node* current = head;
    struct Node* next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}

struct Node* rotate_right(struct Node* head, int k) {
    if (head == NULL || head->next == NULL || k <= 0) {
        return head;
    }

    struct Node* current = head;
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
    for (int i = 0; i < length - k - 1; i++) {
        current = current->next;
    }

    struct Node* new_head = current->next;
    current->next = NULL;

    return new_head;
}

void print_list(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    struct Node* head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(4);
    head->next->next->next->next = create_node(5);

    printf("Original list:\n");
    print_list(head);

    int k = 2;
    head = rotate_right(head, k);

    printf("List after rotating right by %d:\n", k);
    print_list(head);

    free_list(head);

    return 0;
}