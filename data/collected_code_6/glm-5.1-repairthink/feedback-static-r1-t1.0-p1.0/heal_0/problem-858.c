#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void *data;
    int is_list;
    struct Node *next;
} Node;

int count_and_square_lists(Node *head) {
    int count = 0;
    Node *current = head;
    while (current != NULL) {
        if (current->is_list == 1) {
            count++;
        }
        current = current->next;
    }
    return count * count;
}

Node *create_node(void *data, int is_list) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->is_list = is_list;
    node->next = NULL;
    return node;
}

void free_list(Node *head) {
    Node *current = head;
    Node *next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
}

int main() {
    Node *list1 = create_node("item1", 0);
    Node *sublist1 = create_node(NULL, 1);
    Node *list2 = create_node("item2", 0);
    Node *sublist2 = create_node(NULL, 1);
    Node *sublist3 = create_node(NULL, 1);

    if (list1 == NULL || sublist1 == NULL || list2 == NULL || sublist2 == NULL || sublist3 == NULL) {
        free_list(list1);
        free_list(sublist1);
        free_list(list2);
        free_list(sublist2);
        free_list(sublist3);
        return 1;
    }

    list1->next = sublist1;
    sublist1->next = list2;
    list2->next = sublist2;
    sublist2->next = sublist3;

    int squared_count = count_and_square_lists(list1);
    printf("%d\n", squared_count);

    free_list(list1);

    return 0;
}