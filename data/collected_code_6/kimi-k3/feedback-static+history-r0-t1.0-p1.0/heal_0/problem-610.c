#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

int remove_kth_element(Node** head, int k) {
    if (head == NULL || *head == NULL || k < 0) {
        return -1;
    }

    Node* temp = *head;
    Node* prev = NULL;
    int count = 0;

    while (temp != NULL && count < k) {
        prev = temp;
        temp = temp->next;
        count++;
    }

    if (temp == NULL) {
        return -1;
    }

    if (prev == NULL) {
        *head = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    return 0;
}

void print_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void free_list(Node* head) {
    Node* current = head;
    Node* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main(void) {
    Node* head = NULL;
    Node* tail = NULL;
    int values[] = {10, 20, 30, 40, 50};
    int n = sizeof(values) / sizeof(values[0]);
    int i;

    for (i = 0; i < n; i++) {
        Node* new_node = create_node(values[i]);
        if (new_node == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free_list(head);
            return EXIT_FAILURE;
        }
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    printf("Original list: ");
    print_list(head);

    int k = 2;
    if (remove_kth_element(&head, k) == 0) {
        printf("List after removing element at index %d: ", k);
        print_list(head);
    } else {
        printf("Failed to remove element at index %d\n", k);
    }

    free_list(head);
    return EXIT_SUCCESS;
}