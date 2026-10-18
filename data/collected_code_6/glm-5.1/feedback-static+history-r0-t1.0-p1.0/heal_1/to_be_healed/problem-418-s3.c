#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node* create_node(int data) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

Node* find_max_length_sublist(Node *head) {
    Node *max_start = NULL;
    Node *current_start = head;
    int max_len = 0;
    int current_len = 0;

    Node *curr = head;
    Node *prev = NULL;

    while (curr != NULL) {
        if (prev != NULL && curr->data < prev->data) {
            if (current_len > max_len) {
                max_len = current_len;
                max_start = current_start;
            }
            current_start = curr;
            current_len = 1;
        } else {
            current_len++;
        }
        prev = curr;
        curr = curr->next;
    }

    if (current_len > max_len) {
        max_start = current_start;
    }

    return max_start;
}

void print_list(Node *head) {
    Node *curr = head;
    while (curr != NULL) {
        printf("%d ", curr->data);
        curr = curr->next;
    }
    printf("\n");
}

void free_list(Node *head) {
    Node *curr = head;
    while (curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
}

int main() {
    Node *head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(3);
    head->next->next->next = create_node(1);
    head->next->next->next->next = create_node(2);

    /* Possible weaknesses found:
     *  Variable 'max_sublist' can be declared as pointer to const [constVariablePointer]
     */
    Node *max_sublist = find_max_length_sublist(head);

    if (max_sublist != NULL) {
        printf("Max length sublist starts at node with data: %d\n", max_sublist->data);
    } else {
        printf("List is empty.\n");
    }

    free_list(head);

    return 0;
}