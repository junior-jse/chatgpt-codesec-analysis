#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct {
    int *elements;
    int count;
} SubList;

Node *create_node(void *data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

SubList *create_sublist(int *elements, int count) {
    SubList *sublist = (SubList *)malloc(sizeof(SubList));
    if (sublist == NULL) {
        exit(EXIT_FAILURE);
    }
    sublist->elements = elements;
    sublist->count = count;
    return sublist;
}

Node *remove_sublists_outside_range(Node *head, int min_length, int max_length) {
    Node dummy;
    dummy.next = head;
    Node *current = &dummy;

    while (current->next != NULL) {
        /* Possible weaknesses found:
         *  Variable 'sublist' can be declared as pointer to const [constVariablePointer]
         */
        SubList *sublist = (SubList *)current->next->data;
        if (sublist == NULL || sublist->count < min_length || sublist->count > max_length) {
            Node *to_delete = current->next;
            current->next = to_delete->next;
            if (to_delete->data != NULL) {
                SubList *del_sublist = (SubList *)to_delete->data;
                if (del_sublist->elements != NULL) {
                    free(del_sublist->elements);
                }
                free(del_sublist);
            }
            free(to_delete);
        } else {
            current = current->next;
        }
    }

    return dummy.next;
}

void free_list(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        if (current->data != NULL) {
            SubList *sublist = (SubList *)current->data;
            if (sublist->elements != NULL) {
                free(sublist->elements);
            }
            free(sublist);
        }
        free(current);
        current = next;
    }
}

int main(void) {
    int e1[] = {1, 2};
    int e2[] = {3, 4, 5, 6};
    int e3[] = {7, 8, 9};
    int e4[] = {10};

    SubList *s1 = create_sublist(e1, 2);
    SubList *s2 = create_sublist(e2, 4);
    SubList *s3 = create_sublist(e3, 3);
    SubList *s4 = create_sublist(e4, 1);

    Node *head = create_node(s1);
    head->next = create_node(s2);
    head->next->next = create_node(s3);
    head->next->next->next = create_node(s4);

    Node *filtered = remove_sublists_outside_range(head, 2, 3);

    Node *curr = filtered;
    while (curr != NULL) {
        /* Possible weaknesses found:
         *  Variable 'sublist' can be declared as pointer to const [constVariablePointer]
         */
        SubList *sublist = (SubList *)curr->data;
        printf("Sublist count: %d\n", sublist->count);
        curr = curr->next;
    }

    free_list(filtered);

    return 0;
}