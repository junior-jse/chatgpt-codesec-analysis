#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int *data;
    int size;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    int count;
} ListOfLists;

int compare_lists(const int *a, int size_a, const int *b, int size_b) {
    int i;
    if (size_a != size_b) {
        return 0;
    }
    for (i = 0; i < size_a; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

int list_exists(Node *head, const int *data, int size) {
    Node *current = head;
    while (current != NULL) {
        if (compare_lists(current->data, current->size, data, size)) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int count_unique_lists(ListOfLists *list) {
    Node *current;
    Node *unique_head = NULL;
    Node *new_node;
    int unique_count = 0;

    if (list == NULL || list->head == NULL) {
        return 0;
    }

    current = list->head;
    while (current != NULL) {
        if (!list_exists(unique_head, current->data, current->size)) {
            new_node = (Node *)malloc(sizeof(Node));
            if (new_node == NULL) {
                Node *temp;
                while (unique_head != NULL) {
                    temp = unique_head;
                    unique_head = unique_head->next;
                    free(temp);
                }
                return -1;
            }
            new_node->data = current->data;
            new_node->size = current->size;
            new_node->next = unique_head;
            unique_head = new_node;
            unique_count++;
        }
        current = current->next;
    }

    while (unique_head != NULL) {
        Node *temp = unique_head;
        unique_head = unique_head->next;
        free(temp);
    }

    return unique_count;
}

Node *create_list_node(const int *data, int size) {
    int *new_data;
    Node *node;
    
    new_data = (int *)malloc(sizeof(int) * size);
    if (new_data == NULL) {
        return NULL;
    }
    
    memcpy(new_data, data, sizeof(int) * size);
    
    node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        free(new_data);
        return NULL;
    }
    
    node->data = new_data;
    node->size = size;
    node->next = NULL;
    
    return node;
}

void free_list_of_lists(ListOfLists *list) {
    Node *current;
    Node *next;
    
    if (list == NULL) {
        return;
    }
    
    current = list->head;
    while (current != NULL) {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    
    list->head = NULL;
    list->count = 0;
}

int main(void) {
    ListOfLists list;
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5, 6};
    int arr3[] = {1, 2, 3};
    int arr4[] = {7, 8, 9};
    int arr5[] = {4, 5, 6};
    int result;
    
    list.head = NULL;
    list.count = 0;
    
    list.head = create_list_node(arr1, 3);
    if (list.head == NULL) {
        return 1;
    }
    
    list.head->next = create_list_node(arr2, 3);
    if (list.head->next == NULL) {
        free_list_of_lists(&list);
        return 1;
    }
    
    list.head->next->next = create_list_node(arr3, 3);
    if (list.head->next->next == NULL) {
        free_list_of_lists(&list);
        return 1;
    }
    
    list.head->next->next->next = create_list_node(arr4, 3);
    if (list.head->next->next->next == NULL) {
        free_list_of_lists(&list);
        return 1;
    }
    
    list.head->next->next->next->next = create_list_node(arr5, 3);
    if (list.head->next->next->next->next == NULL) {
        free_list_of_lists(&list);
        return 1;
    }
    
    result = count_unique_lists(&list);
    
    if (result >= 0) {
        printf("Unique lists: %d\n", result);
    }
    
    free_list_of_lists(&list);
    
    return 0;
}