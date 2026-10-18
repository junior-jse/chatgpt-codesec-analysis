#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int *data;
    size_t size;
    struct Node *next;
} Node;

Node *create_node(const int *arr, size_t size) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = malloc(size * sizeof(int));
    if (node->data == NULL) {
        free(node);
        return NULL;
    }
    memcpy(node->data, arr, size * sizeof(int));
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
    size_t capacity = 16;
    size_t count = 0;
    int *result = malloc(capacity * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    const Node *current = nested;
    while (current != NULL) {
        for (size_t i = 0; i < current->size; i++) {
            int found = 0;
            for (size_t j = 0; j < list_size; j++) {
                if (current->data[i] == list[j]) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                int duplicate = 0;
                for (size_t k = 0; k < count; k++) {
                    if (result[k] == current->data[i]) {
                        duplicate = 1;
                        break;
                    }
                }
                if (!duplicate) {
                    if (count >= capacity) {
                        size_t new_capacity = capacity * 2;
                        int *new_result = realloc(result, new_capacity * sizeof(int));
                        if (new_result == NULL) {
                            free(result);
                            return NULL;
                        }
                        result = new_result;
                        capacity = new_capacity;
                    }
                    result[count++] = current->data[i];
                }
            }
        }
        current = current->next;
    }

    if (count == 0) {
        free(result);
        *result_size = 0;
        return NULL;
    }

    int *final_result = realloc(result, count * sizeof(int));
    if (final_result == NULL) {
        *result_size = count;
        return result;
    }

    *result_size = count;
    return final_result;
}

int main(void) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5, 6};
    int arr3[] = {7, 8, 9};
    int flat_list[] = {2, 3, 5, 7, 10, 11};

    Node *head = create_node(arr1, 3);
    if (head == NULL) {
        return EXIT_FAILURE;
    }

    Node *second = create_node(arr2, 3);
    if (second == NULL) {
        free_list(head);
        return EXIT_FAILURE;
    }

    Node *third = create_node(arr3, 3);
    if (third == NULL) {
        free_list(head);
        free_list(second);
        return EXIT_FAILURE;
    }

    head->next = second;
    second->next = third;

    size_t result_size = 0;
    int *common = find_common_elements(head, flat_list, 6, &result_size);
    
    if (common != NULL) {
        printf("Common elements: ");
        for (size_t i = 0; i < result_size; i++) {
            printf("%d ", common[i]);
        }
        printf("\n");
        free(common);
    } else if (result_size == 0) {
        printf("No common elements found.\n");
    } else {
        free_list(head);
        return EXIT_FAILURE;
    }

    free_list(head);
    return EXIT_SUCCESS;
}