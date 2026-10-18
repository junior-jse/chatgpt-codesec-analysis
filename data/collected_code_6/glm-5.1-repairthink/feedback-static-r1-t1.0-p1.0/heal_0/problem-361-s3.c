#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

Node *remove_empty_lists(Node *head) {
    Node **current = &head;
    while (*current != NULL) {
        Node *entry = *current;
        Node *inner_head = (Node *)entry->data;
        if (inner_head == NULL) {
            *current = entry->next;
            free(entry);
        } else {
            current = &entry->next;
        }
    }
    return head;
}

Node *create_node(void *data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        if (temp->data != NULL) {
            Node *inner = (Node *)temp->data;
            while (inner != NULL) {
                Node *inner_temp = inner;
                inner = inner->next;
                free(inner_temp);
            }
        }
        free(temp);
    }
}

int main(void) {
    Node *inner1 = create_node((void *)(long)1);
    Node *inner3 = create_node((void *)(long)3);

    Node *list1 = create_node(inner1);
    list1->next = create_node(NULL);
    list1->next->next = create_node(inner3);
    list1->next->next->next = create_node(NULL);

    list1 = remove_empty_lists(list1);

    Node *curr = list1;
    while (curr != NULL) {
        Node *inner = (Node *)curr->data;
        printf("List (data: %ld)\n", (long)inner->data);
        curr = curr->next;
    }

    free_list(list1);
    return 0;
}