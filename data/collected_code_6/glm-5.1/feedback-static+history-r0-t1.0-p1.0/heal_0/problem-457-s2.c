#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node** sublists;
    int* lengths;
    int count;
} SublistArray;

SublistArray* find_sublists_of_minimum_length(Node** heads, int num_lists) {
    if (heads == NULL || num_lists <= 0) {
        return NULL;
    }

    int min_len = 0;
    int* lengths = (int*)malloc(num_lists * sizeof(int));
    if (lengths == NULL) {
        return NULL;
    }

    for (int i = 0; i < num_lists; i++) {
        int len = 0;
        Node* current = heads[i];
        while (current != NULL) {
            len++;
            current = current->next;
        }
        lengths[i] = len;
        if (i == 0 || len < min_len) {
            min_len = len;
        }
    }

    int min_count = 0;
    for (int i = 0; i < num_lists; i++) {
        if (lengths[i] == min_len) {
            min_count++;
        }
    }

    if (min_count == 0) {
        free(lengths);
        return NULL;
    }

    SublistArray* result = (SublistArray*)malloc(sizeof(SublistArray));
    if (result == NULL) {
        free(lengths);
        return NULL;
    }

    result->sublists = (Node**)malloc(min_count * sizeof(Node*));
    if (result->sublists == NULL) {
        free(lengths);
        free(result);
        return NULL;
    }

    result->lengths = (int*)malloc(min_count * sizeof(int));
    if (result->lengths == NULL) {
        free(lengths);
        free(result->sublists);
        free(result);
        return NULL;
    }

    result->count = min_count;
    int idx = 0;
    for (int i = 0; i < num_lists; i++) {
        if (lengths[i] == min_len) {
            result->sublists[idx] = heads[i];
            result->lengths[idx] = lengths[i];
            idx++;
        }
    }

    free(lengths);
    return result;
}

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node != NULL) {
        new_node->data = data;
        new_node->next = NULL;
    }
    return new_node;
}

void free_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    Node* list1 = create_node(1);
    if (list1 != NULL) {
        list1->next = create_node(2);
    }

    Node* list2 = create_node(3);

    Node* list3 = create_node(4);
    if (list3 != NULL) {
        list3->next = create_node(5);
    }

    Node* list4 = create_node(6);
    if (list4 != NULL) {
        list4->next = create_node(7);
        if (list4->next != NULL) {
            list4->next->next = create_node(8);
        }
    }

    Node* heads[4] = {list1, list2, list3, list4};
    int num_lists = 4;

    SublistArray* min_sublists = find_sublists_of_minimum_length(heads, num_lists);

    if (min_sublists != NULL) {
        printf("Minimum length: %d\n", min_sublists->lengths[0]);
        for (int i = 0; i < min_sublists->count; i++) {
            printf("Sublist %d: ", i);
            Node* current = min_sublists->sublists[i];
            while (current != NULL) {
                printf("%d ", current->data);
                current = current->next;
            }
            printf("\n");
        }
        free(min_sublists->lengths);
        free(min_sublists->sublists);
        free(min_sublists);
    } else {
        printf("No sublists found or memory allocation failed.\n");
    }

    free_list(list1);
    free_list(list2);
    free_list(list3);
    free_list(list4);

    return 0;
}