#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct {
    int size;
} ListMeta;

Node *remove_empty_lists(Node *head) {
    Node dummy;
    dummy.next = head;
    Node *prev = &dummy;
    Node *curr = head;

    while (curr != NULL) {
        const ListMeta *meta = (const ListMeta *)curr->data;
        if (meta != NULL && meta->size == 0) {
            prev->next = curr->next;
            Node *temp = curr;
            curr = curr->next;
            free(temp->data);
            free(temp);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }

    return dummy.next;
}

Node *create_node(void *data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

void free_list(Node *head) {
    Node *curr = head;
    while (curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp->data);
        free(temp);
    }
}

int main(void) {
    ListMeta *m1 = (ListMeta *)malloc(sizeof(ListMeta));
    ListMeta *m2 = (ListMeta *)malloc(sizeof(ListMeta));
    ListMeta *m3 = (ListMeta *)malloc(sizeof(ListMeta));
    ListMeta *m4 = (ListMeta *)malloc(sizeof(ListMeta));

    m1->size = 5;
    m2->size = 0;
    m3->size = 0;
    m4->size = 3;

    Node *head = create_node(m1);
    if (head == NULL) {
        free(m1); free(m2); free(m3); free(m4);
        return 1;
    }
    head->next = create_node(m2);
    if (head->next == NULL) {
        free(m2); free(m3); free(m4); free_list(head);
        return 1;
    }
    head->next->next = create_node(m3);
    if (head->next->next == NULL) {
        free(m3); free(m4); free_list(head);
        return 1;
    }
    head->next->next->next = create_node(m4);
    if (head->next->next->next == NULL) {
        free(m4); free_list(head);
        return 1;
    }

    Node *curr = head;
    while (curr != NULL) {
        printf("%d ", ((const ListMeta *)curr->data)->size);
        curr = curr->next;
    }
    printf("\n");

    head = remove_empty_lists(head);

    curr = head;
    while (curr != NULL) {
        printf("%d ", ((const ListMeta *)curr->data)->size);
        curr = curr->next;
    }
    printf("\n");

    free_list(head);

    return 0;
}