#include <stdlib.h>
#include <stdio.h>

struct Node {
    int value;
    struct Node *next;
};

struct Sublist {
    struct Node *head;
    struct Sublist *next;
};

struct Node *create_node(int value) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (node == NULL) {
        exit(EXIT_FAILURE);
    }
    node->value = value;
    node->next = NULL;
    return node;
}

struct Sublist *create_sublist(struct Node *head) {
    struct Sublist *sublist = (struct Sublist *)malloc(sizeof(struct Sublist));
    if (sublist == NULL) {
        exit(EXIT_FAILURE);
    }
    sublist->head = head;
    sublist->next = NULL;
    return sublist;
}

int *get_last_elements(struct Sublist *head, size_t *out_size) {
    if (head == NULL || out_size == NULL) {
        return NULL;
    }

    size_t count = 0;
    struct Sublist *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    int *results = (int *)malloc(count * sizeof(int));
    if (results == NULL) {
        return NULL;
    }

    current = head;
    for (size_t i = 0; i < count; i++) {
        if (current->head == NULL) {
            free(results);
            return NULL;
        }
        struct Node *node = current->head;
        while (node->next != NULL) {
            node = node->next;
        }
        results[i] = node->value;
        current = current->next;
    }

    *out_size = count;
    return results;
}

void free_list(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *next = current->next;
        free(current);
        current = next;
    }
}

void free_sublists(struct Sublist *head) {
    struct Sublist *current = head;
    while (current != NULL) {
        struct Sublist *next = current->next;
        free_list(current->head);
        free(current);
        current = next;
    }
}

int main(void) {
    struct Node *list1 = create_node(1);
    list1->next = create_node(2);
    list1->next->next = create_node(3);

    struct Node *list2 = create_node(4);
    list2->next = create_node(5);

    struct Node *list3 = create_node(6);

    struct Sublist *sublists = create_sublist(list1);
    sublists->next = create_sublist(list2);
    sublists->next->next = create_sublist(list3);

    size_t size = 0;
    int *last_elements = get_last_elements(sublists, &size);

    if (last_elements != NULL) {
        for (size_t i = 0; i < size; i++) {
            printf("%d\n", last_elements[i]);
        }
        free(last_elements);
    }

    free_sublists(sublists);

    return 0;
}