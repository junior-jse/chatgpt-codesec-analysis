#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *next;
};

struct Node *create_node(int data) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

void free_list(struct Node *head) {
    struct Node *current = head;
    struct Node *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

struct Node *divide_lists(struct Node *a, struct Node *b) {
    struct Node *result = NULL;
    struct Node **tail = &result;
    
    while (a != NULL && b != NULL) {
        if (b->data == 0) {
            free_list(result);
            exit(EXIT_FAILURE);
        }
        struct Node *node = create_node(a->data / b->data);
        *tail = node;
        tail = &(node->next);
        a = a->next;
        b = b->next;
    }
    
    return result;
}

void append(struct Node **head, int data) {
    struct Node *node = create_node(data);
    if (*head == NULL) {
        *head = node;
    } else {
        struct Node *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }
}

void print_list(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    struct Node *list1 = NULL;
    struct Node *list2 = NULL;
    
    append(&list1, 10);
    append(&list1, 20);
    append(&list1, 30);
    append(&list1, 40);
    
    append(&list2, 2);
    append(&list2, 4);
    append(&list2, 5);
    append(&list2, 8);

    struct Node *divided = divide_lists(list1, list2);
    print_list(divided);

    free_list(list1);
    free_list(list2);
    free_list(divided);
    
    return 0;
}