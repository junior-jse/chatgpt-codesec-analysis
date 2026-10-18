#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

Node *remove_empty_lists(Node *head) {
    Node dummy;
    dummy.next = head;
    Node *prev = &dummy;
    Node *curr = head;

    while (curr != NULL) {
        const Node *sub_list = (const Node *)curr->data;
        if (sub_list == NULL) {
            prev->next = curr->next;
            Node *to_free = curr;
            curr = curr->next;
            free(to_free);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }

    return dummy.next;
}

Node *create_node(void *data) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (n == NULL) {
        exit(EXIT_FAILURE);
    }
    n->data = data;
    n->next = NULL;
    return n;
}

Node *create_sublist(int *vals, int size) {
    Node *head = NULL;
    Node *tail = NULL;
    for (int i = 0; i < size; i++) {
        Node *n = create_node((void *)&vals[i]);
        if (head == NULL) {
            head = n;
            tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
    }
    return head;
}

void free_all(Node *head) {
    while (head != NULL) {
        Node *sub = (Node *)head->data;
        Node *next = head->next;
        while (sub != NULL) {
            Node *sub_next = sub->next;
            free(sub);
            sub = sub_next;
        }
        free(head);
        head = next;
    }
}

int main() {
    int v1[] = {1, 2};
    int v2[] = {3};

    Node *l1 = create_sublist(v1, 2);
    Node *l2 = create_sublist(NULL, 0);
    Node *l3 = create_sublist(v2, 1);
    Node *l4 = create_sublist(NULL, 0);

    Node *outer1 = create_node((void *)l1);
    Node *outer2 = create_node((void *)l2);
    Node *outer3 = create_node((void *)l3);
    Node *outer4 = create_node((void *)l4);

    outer1->next = outer2;
    outer2->next = outer3;
    outer3->next = outer4;

    Node *result = remove_empty_lists(outer1);

    int count = 0;
    Node *curr = result;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }

    printf("%d\n", count);

    free_all(result);

    return 0;
}