#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NestedNode {
    int value;
    struct NestedNode *next;
} NestedNode;

typedef struct {
    NestedNode *head;
    int id;
} NestedList;

typedef struct {
    NestedList *lists;
    size_t count;
} NestedListArray;

NestedNode* create_node(int value) {
    NestedNode *node = (NestedNode*)malloc(sizeof(NestedNode));
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    return node;
}

void free_nested_list(NestedNode *head) {
    NestedNode *current = head;
    while (current != NULL) {
        NestedNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void free_nested_array(NestedListArray *arr) {
    if (arr == NULL) {
        return;
    }
    if (arr->lists != NULL) {
        for (size_t i = 0; i < arr->count; i++) {
            free_nested_list(arr->lists[i].head);
        }
        free(arr->lists);
        arr->lists = NULL;
    }
    arr->count = 0;
}

int value_exists_in_list(const NestedNode *head, int value) {
    const NestedNode *current = head;
    while (current != NULL) {
        if (current->value == value) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int value_exists_in_nested(const NestedListArray *nested, int value) {
    if (nested == NULL || nested->lists == NULL) {
        return 0;
    }
    for (size_t i = 0; i < nested->count; i++) {
        if (value_exists_in_list(nested->lists[i].head, value)) {
            return 1;
        }
    }
    return 0;
}

int* find_common_elements(const NestedListArray *nested, const int *flat_list, 
                          size_t flat_size, size_t *result_size) {
    if (nested == NULL || flat_list == NULL || result_size == NULL || flat_size == 0) {
        if (result_size != NULL) {
            *result_size = 0;
        }
        return NULL;
    }

    int *result = (int*)malloc(flat_size * sizeof(int));
    if (result == NULL) {
        *result_size = 0;
        return NULL;
    }

    size_t count = 0;
    for (size_t i = 0; i < flat_size; i++) {
        if (value_exists_in_nested(nested, flat_list[i])) {
            int already_added = 0;
            for (size_t j = 0; j < count; j++) {
                if (result[j] == flat_list[i]) {
                    already_added = 1;
                    break;
                }
            }
            if (!already_added) {
                result[count] = flat_list[i];
                count++;
            }
        }
    }

    if (count == 0) {
        free(result);
        *result_size = 0;
        return NULL;
    }

    int *final_result = (int*)realloc(result, count * sizeof(int));
    if (final_result == NULL) {
        free(result);
        *result_size = 0;
        return NULL;
    }

    *result_size = count;
    return final_result;
}

int add_to_nested_list(NestedList *list, int value, int id) {
    if (list == NULL) {
        return 0;
    }
    NestedNode *new_node = create_node(value);
    if (new_node == NULL) {
        return 0;
    }
    if (list->head == NULL) {
        list->head = new_node;
        list->id = id;
    } else {
        NestedNode *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    return 1;
}

int main(void) {
    NestedListArray nested = {0};
    nested.count = 3;
    nested.lists = (NestedList*)calloc(nested.count, sizeof(NestedList));
    
    if (nested.lists == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < nested.count; i++) {
        nested.lists[i].head = NULL;
        nested.lists[i].id = (int)i;
    }

    if (!add_to_nested_list(&nested.lists[0], 1, 0) ||
        !add_to_nested_list(&nested.lists[0], 2, 0) ||
        !add_to_nested_list(&nested.lists[0], 3, 0) ||
        !add_to_nested_list(&nested.lists[1], 4, 1) ||
        !add_to_nested_list(&nested.lists[1], 5, 1) ||
        !add_to_nested_list(&nested.lists[2], 6, 2) ||
        !add_to_nested_list(&nested.lists[2], 7, 2) ||
        !add_to_nested_list(&nested.lists[2], 8, 2)) {
        fprintf(stderr, "Failed to build nested lists\n");
        free_nested_array(&nested);
        return EXIT_FAILURE;
    }

    int flat_list[] = {2, 4, 6, 9, 10, 3, 8};
    size_t flat_size = sizeof(flat_list) / sizeof(flat_list[0]);
    size_t result_size = 0;

    int *common = find_common_elements(&nested, flat_list, flat_size, &result_size);

    if (common != NULL) {
        printf("Common elements: ");
        for (size_t i = 0; i < result_size; i++) {
            printf("%d ", common[i]);
        }
        printf("\n");
        free(common);
    } else {
        printf("No common elements found or error occurred\n");
    }

    free_nested_array(&nested);
    return EXIT_SUCCESS;
}