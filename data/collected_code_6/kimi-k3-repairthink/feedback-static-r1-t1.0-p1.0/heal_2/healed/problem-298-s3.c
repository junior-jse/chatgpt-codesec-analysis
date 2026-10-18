#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct Node {
    int *data;
    size_t size;
    struct Node *next;
} Node;

Node *create_node(const int *arr, size_t size) {
    if (arr == NULL || size == 0) {
        return NULL;
    }
    if (size > SIZE_MAX / sizeof(int)) {
        return NULL;
    }
    Node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    size_t bytes = size * sizeof(int);
    node->data = malloc(bytes);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }
    for (size_t i = 0; i < size; i++) {
        node->data[i] = arr[i];
    }
    node->size = size;
    node->next = NULL;
    return node;
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp->data);
        free(temp);
    }
}

int *find_common_elements(const Node *nested, const int *list, size_t list_size, size_t *result_size) {
    if (result_size == NULL) {
        return NULL;
    }
    *result_size = 0;

    if (list == NULL && list_size > 0) {
        return NULL;
    }

    size_t capacity = 16;
    size_t count = 0;

    if (capacity > SIZE_MAX / sizeof(int)) {
        return NULL;
    }

    int *result = malloc(capacity * sizeof(*result));
    if (result == NULL) {
        return NULL;
    }

    const Node *current = nested;
    while (current != NULL) {
        if (current->data == NULL) {
            current = current->next;
            continue;
        }
        for (size_t i = 0; i < current->size; i++) {
            int found = 0;
            for (size_t j = 0; j < list_size; j++) {
                if (current->data[i] == list[j]) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                continue;
            }
            int duplicate = 0;
            for (size_t k = 0; k < count; k++) {
                if (result[k] == current->data[i]) {
                    duplicate = 1;
                    break;
                }
            }
            if (duplicate) {
                continue;
            }
            if (count >= capacity) {
                if (capacity > SIZE_MAX / 2) {
                    free(result);
                    return NULL;
                }
                size_t new_capacity = capacity * 2;
                if (new_capacity > SIZE_MAX / sizeof(*result)) {
                    free(result);
                    return NULL;
                }
                int *new_result = realloc(result, new_capacity * sizeof(*new_result));
                if (new_result == NULL) {
                    free(result);
                    return NULL;
                }
                result = new_result;
                capacity = new_capacity;
            }
            result[count++] = current->data[i];
        }
        current = current->next;
    }

    if (count == 0) {
        free(result);
        return NULL;
    }

    int *final_result = realloc(result, count * sizeof(*final_result));
    if (final_result != NULL) {
        result = final_result;
    }

    *result_size = count;
    return result;
}

int main(void) {
    const int arr1[] = {1, 2, 3};
    const int arr2[] = {4, 5, 6};
    const int arr3[] = {7, 8, 9};
    const int flat_list[] = {2, 3, 5, 7, 10, 11};

    Node *head = create_node(arr1, sizeof(arr1) / sizeof(arr1[0]));
    if (head == NULL) {
        return EXIT_FAILURE;
    }

    Node *second = create_node(arr2, sizeof(arr2) / sizeof(arr2[0]));
    if (second == NULL) {
        free_list(head);
        return EXIT_FAILURE;
    }
    head->next = second;

    Node *third = create_node(arr3, sizeof(arr3) / sizeof(arr3[0]));
    if (third == NULL) {
        free_list(head);
        return EXIT_FAILURE;
    }
    second->next = third;

    size_t result_size = 0;
    int *common = find_common_elements(head, flat_list, sizeof(flat_list) / sizeof(flat_list[0]), &result_size);

    if (common != NULL) {
        printf("Common elements: ");
        for (size_t i = 0; i < result_size; i++) {
            printf("%d ", common[i]);
        }
        printf("\n");
        free(common);
    } else {
        printf("No common elements found.\n");
    }

    free_list(head);
    return EXIT_SUCCESS;
}