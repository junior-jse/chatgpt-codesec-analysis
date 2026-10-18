#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct Node {
    int *data;
    int size;
    struct Node *next;
} Node;

Node *create_node(const int *data, int size) {
    if (size < 0) {
        return NULL;
    }

    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        return NULL;
    }

    if (size > 0 && data != NULL) {
        size_t alloc_size = (size_t)size * sizeof(int);
        if (alloc_size == 0 || (alloc_size / sizeof(int)) != (size_t)size) {
            free(new_node);
            return NULL;
        }
        new_node->data = (int *)malloc(alloc_size);
        if (!new_node->data) {
            free(new_node);
            return NULL;
        }
        if (alloc_size <= SIZE_MAX) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(new_node->data, data, alloc_size);
        } else {
            free(new_node->data);
            free(new_node);
            return NULL;
        }
    } else {
        new_node->data = NULL;
    }

    new_node->size = size;
    new_node->next = NULL;
    return new_node;
}

void free_list(Node *head) {
    Node *current = head;
    while (current) {
        Node *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

int is_within_range(const int *sublist, int size, int min_val, int max_val) {
    if (sublist == NULL) {
        return 0;
    }
    for (int i = 0; i < size; i++) {
        if (sublist[i] < min_val || sublist[i] > max_val) {
            return 0;
        }
    }
    return 1;
}

Node *remove_out_of_range(Node *head, int min_val, int max_val) {
    Node dummy;
    dummy.next = head;
    Node *prev = &dummy;
    Node *current = head;

    while (current) {
        Node *next = current->next;
        if (!is_within_range(current->data, current->size, min_val, max_val)) {
            prev->next = next;
            free(current->data);
            free(current);
        } else {
            prev = current;
        }
        current = next;
    }

    return dummy.next;
}

void print_list(Node *head) {
    Node *current = head;
    while (current) {
        printf("[");
        for (int i = 0; i < current->size; i++) {
            printf("%d", current->data[i]);
            if (i < current->size - 1) {
                printf(", ");
            }
        }
        printf("] ");
        current = current->next;
    }
    printf("\n");
}

int main(void) {
    const int arr1[] = {1, 2, 3};
    const int arr2[] = {4, 5, 6};
    const int arr3[] = {7, 8, 9};
    const int arr4[] = {10, 11, 12};

    Node *head = create_node(arr1, 3);
    if (!head) return 1;
    head->next = create_node(arr2, 3);
    if (!head->next) {
        free_list(head);
        return 1;
    }
    head->next->next = create_node(arr3, 3);
    if (!head->next->next) {
        free_list(head);
        return 1;
    }
    head->next->next->next = create_node(arr4, 3);
    if (!head->next->next->next) {
        free_list(head);
        return 1;
    }

    printf("Original list: ");
    print_list(head);

    head = remove_out_of_range(head, 1, 9);

    printf("Filtered list: ");
    print_list(head);

    free_list(head);

    return 0;
}