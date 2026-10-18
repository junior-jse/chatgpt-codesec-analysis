#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *remove_consecutive_duplicates(Node *head) {
    Node *current;
    Node *temp;

    if (head == NULL) {
        return NULL;
    }

    current = head;
    while (current != NULL && current->next != NULL) {
        if (current->data == current->next->data) {
            temp = current->next;
            current->next = current->next->next;
            free(temp);
            temp = NULL;
        } else {
            current = current->next;
        }
    }

    return head;
}

Node *create_node(int data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_list(Node *head) {
    /* Possible weaknesses found:
     *  The scope of the variable 'temp' can be reduced. [variableScope]
     */
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void print_list(const Node *head) {
    const Node *current = head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    Node *head = NULL;
    Node *tail = NULL;
    Node *new_node = NULL;
    int values[] = {1, 2, 2, 3, 3, 3, 4, 5, 5, 6};
    size_t i;
    size_t count = sizeof(values) / sizeof(values[0]);

    for (i = 0; i < count; i++) {
        new_node = create_node(values[i]);
        if (new_node == NULL) {
            free_list(head);
            fprintf(stderr, "Memory allocation failed\n");
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

    head = remove_consecutive_duplicates(head);

    printf("After removing consecutive duplicates: ");
    print_list(head);

    free_list(head);
    head = NULL;

    return EXIT_SUCCESS;
}