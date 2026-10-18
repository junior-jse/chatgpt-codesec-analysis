#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct Group {
    int key;
    Node *head;
    struct Group *next;
} Group;

Group *find_group(Group *head, int key) {
    Group *current = head;
    while (current != NULL) {
        if (current->key == key) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Group *add_to_group(Group *head, int key, int value) {
    Group *grp = find_group(head, key);
    if (grp == NULL) {
        grp = (Group *)malloc(sizeof(Group));
        if (grp == NULL) {
            return head;
        }
        grp->key = key;
        grp->head = NULL;
        grp->next = head;
        head = grp;
    }
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return head;
    }
    new_node->value = value;
    new_node->next = grp->head;
    grp->head = new_node;
    return head;
}

void free_groups(Group *head) {
    Group *current_grp = head;
    while (current_grp != NULL) {
        Group *next_grp = current_grp->next;
        Node *current_node = current_grp->head;
        while (current_node != NULL) {
            Node *next_node = current_node->next;
            free(current_node);
            current_node = next_node;
        }
        free(current_grp);
        current_grp = next_grp;
    }
}

void print_groups(Group *head) {
    Group *current_grp = head;
    while (current_grp != NULL) {
        printf("%d: [", current_grp->key);
        Node *current_node = current_grp->head;
        while (current_node != NULL) {
            printf("%d", current_node->value);
            if (current_node->next != NULL) {
                printf(", ");
            }
            current_node = current_node->next;
        }
        printf("]\n");
        current_grp = current_grp->next;
    }
}

Group *group_tuples(Tuple *tuples, size_t size) {
    Group *head = NULL;
    for (size_t i = 0; i < size; i++) {
        head = add_to_group(head, tuples[i].second, tuples[i].first);
    }
    return head;
}

int main(void) {
    Tuple tuples[] = {{1, 2}, {3, 2}, {4, 5}, {6, 5}, {7, 8}};
    size_t size = sizeof(tuples) / sizeof(tuples[0]);

    Group *groups = group_tuples(tuples, size);
    print_groups(groups);
    free_groups(groups);

    return 0;
}