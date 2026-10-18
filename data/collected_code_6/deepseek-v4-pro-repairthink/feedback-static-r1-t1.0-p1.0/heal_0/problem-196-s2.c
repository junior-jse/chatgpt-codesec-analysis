#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void append_node(Node **head, int data) {
    Node *new_node = create_node(data);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_node;
}

Node *remove_tuples_of_length_k(Node *head, int k) {
    if (head == NULL || k <= 0) {
        return head;
    }

    Node *dummy = create_node(0);
    dummy->next = head;
    Node *prev = dummy;
    Node *current = head;
    int count = 1;
    Node *tuple_start = current;
    Node *tuple_start_prev = dummy;

    while (current != NULL) {
        if (count == k) {
            Node *next_node = current->next;
            Node *temp = tuple_start;
            while (temp != next_node) {
                Node *to_free = temp;
                temp = temp->next;
                free(to_free);
            }
            tuple_start_prev->next = next_node;
            current = next_node;
            count = 1;
            tuple_start = current;
            tuple_start_prev = (current == NULL) ? NULL : prev;
            prev = tuple_start_prev;
            if (current == NULL) {
                break;
            }
        } else {
            count++;
            prev = current;
            current = current->next;
        }
    }

    Node *result = dummy->next;
    free(dummy);
    return result;
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
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
    Node *head = NULL;
    int test_data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int data_length = sizeof(test_data) / sizeof(test_data[0]);
    int k = 3;

    for (int i = 0; i < data_length; i++) {
        append_node(&head, test_data[i]);
    }

    printf("Original list: ");
    print_list(head);

    head = remove_tuples_of_length_k(head, k);

    printf("List after removing tuples of length %d: ", k);
    print_list(head);

    free_list(head);
    return 0;
}