#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

int count_sublists(struct Node *head, int element) {
    struct Node *current = head;
    struct Node *sublist_head = NULL;
    int count = 0;

    while (current != NULL) {
        if (current->data == element && sublist_head == NULL) {
            sublist_head = current;
            if (sublist_head == head) {
                count++;
            }
        }

        if (current->data != element && sublist_head != NULL) {
            count++;
            sublist_head = NULL;
        }

        current = current->next;
    }

    if (sublist_head != NULL) {
        count++;
    }

    return count;
}

struct Node *create_node(int data) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_list(struct Node *head) {
    struct Node *current = head;
    struct Node *next_node = NULL;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}

int main(void) {
    struct Node *head = create_node(1);
    head->next = create_node(2);
    head->next->next = create_node(2);
    head->next->next->next = create_node(3);
    head->next->next->next->next = create_node(2);
    head->next->next->next->next->next = create_node(2);
    head->next->next->next->next->next->next = create_node(4);

    int target = 2;
    int result = count_sublists(head, target);

    printf("%d\n", result);

    free_list(head);

    return 0;
}