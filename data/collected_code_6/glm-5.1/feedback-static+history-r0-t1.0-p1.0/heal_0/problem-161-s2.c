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

Node *append(Node *head, int data) {
    Node *node = create_node(data);
    if (head == NULL) {
        return node;
    }
    Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = node;
    return head;
}

int is_present(Node *head, int data) {
    Node *current = head;
    while (current != NULL) {
        if (current->data == data) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

Node *remove_elements(Node *list1, Node *list2) {
    Node dummy;
    dummy.next = list1;
    Node *prev = &dummy;
    Node *current = list1;

    while (current != NULL) {
        if (is_present(list2, current->data)) {
            Node *temp = current;
            prev->next = current->next;
            current = current->next;
            free(temp);
        } else {
            prev = current;
            current = current->next;
        }
    }

    return dummy.next;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
}

void print_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    Node *list1 = NULL;
    Node *list2 = NULL;

    list1 = append(list1, 1);
    list1 = append(list1, 2);
    list1 = append(list1, 3);
    list1 = append(list1, 4);
    list1 = append(list1, 5);

    list2 = append(list2, 2);
    list2 = append(list2, 4);

    list1 = remove_elements(list1, list2);

    print_list(list1);

    free_list(list1);
    free_list(list2);

    return 0;
}