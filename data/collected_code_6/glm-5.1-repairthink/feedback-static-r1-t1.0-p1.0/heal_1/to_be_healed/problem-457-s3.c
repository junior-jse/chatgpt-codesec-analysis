#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

void append_node(Node **head, int data) {
    if (head == NULL) {
        return;
    }
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

Node *find_min_sublist(Node *head) {
    if (head == NULL) {
        return NULL;
    }

    Node *min_start = NULL;
    size_t min_length = 0;
    int is_descending = 0;

    Node *current_start = head;
    Node *prev = head;
    size_t current_length = 1;
    is_descending = 0;

    while (prev != NULL && prev->next != NULL) {
        Node *next_node = prev->next;
        if (prev->data < next_node->data && !is_descending) {
            current_length++;
        } else if (prev->data > next_node->data) {
            if (!is_descending && current_length > 1) {
                is_descending = 1;
                current_length++;
            } else if (is_descending) {
                current_length++;
            } else {
                current_start = next_node;
                current_length = 1;
                is_descending = 0;
            }
        } else {
            if (min_start == NULL || current_length < min_length) {
                min_length = current_length;
                min_start = current_start;
            }
            current_start = next_node;
            current_length = 1;
            is_descending = 0;
        }
        prev = next_node;
    }

    if (min_start == NULL || current_length < min_length) {
        /* Possible weaknesses found:
         *  Variable 'min_length' is assigned a value that is never used. [unreadVariable]
         */
        min_length = current_length;
        min_start = current_start;
    }

    if (current_start == head && head->next != NULL && min_start == head) {
        return head;
    }

    Node *search = head;
    while (search != NULL && search->next != min_start) {
        search = search->next;
    }
    if (search != NULL) {
        search->next = NULL;
    }

    return min_start;
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
    append_node(&head, 10);
    append_node(&head, 20);
    append_node(&head, 15);
    append_node(&head, 5);
    append_node(&head, 50);
    append_node(&head, 60);
    append_node(&head, 55);

    Node *sublist = find_min_sublist(head);

    Node *current = sublist;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");

    free_list(sublist);
    free_list(head);

    return 0;
}