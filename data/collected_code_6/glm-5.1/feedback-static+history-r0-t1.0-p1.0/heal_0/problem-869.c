#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct Listnode {
    Node *sublist;
    struct Listnode *next;
} Listnode;

Node *create_node(int value) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        exit(EXIT_FAILURE);
    }
    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}

Listnode *create_listnode(Node *sublist) {
    Listnode *new_listnode = (Listnode *)malloc(sizeof(Listnode));
    if (new_listnode == NULL) {
        exit(EXIT_FAILURE);
    }
    new_listnode->sublist = sublist;
    new_listnode->next = NULL;
    return new_listnode;
}

void free_sublist(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
}

Listnode *remove_out_of_range(Listnode *head, int lower, int upper) {
    Listnode dummy;
    dummy.next = head;
    Listnode *prev = &dummy;
    Listnode *current = head;

    while (current != NULL) {
        Node *sub_head = current->sublist;
        if (sub_head == NULL || sub_head->data < lower || sub_head->data > upper) {
            prev->next = current->next;
            free_sublist(current->sublist);
            free(current);
            current = prev->next;
        } else {
            prev = current;
            current = current->next;
        }
    }

    return dummy.next;
}

void print_lists(Listnode *head) {
    Listnode *current = head;
    while (current != NULL) {
        Node *sub_current = current->sublist;
        printf("[");
        while (sub_current != NULL) {
            printf("%d", sub_current->data);
            if (sub_current->next != NULL) {
                printf(", ");
            }
            sub_current = sub_current->next;
        }
        printf("]");
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

void free_lists(Listnode *head) {
    Listnode *current = head;
    while (current != NULL) {
        Listnode *next = current->next;
        free_sublist(current->sublist);
        free(current);
        current = next;
    }
}

int main() {
    Node *s1 = create_node(5);
    s1->next = create_node(10);
    Node *s2 = create_node(15);
    Node *s3 = create_node(2);
    s3->next = create_node(4);
    Node *s4 = create_node(25);
    Node *s5 = create_node(12);
    s5->next = create_node(18);

    Listnode *head = create_listnode(s1);
    head->next = create_listnode(s2);
    head->next->next = create_listnode(s3);
    head->next->next->next = create_listnode(s4);
    head->next->next->next->next = create_listnode(s5);

    printf("Original list of lists:\n");
    print_lists(head);

    int lower = 5;
    int upper = 20;
    printf("Removing sublists outside range [%d, %d]\n", lower, upper);
    head = remove_out_of_range(head, lower, upper);

    printf("Modified list of lists:\n");
    print_lists(head);

    free_lists(head);
    return 0;
}