#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct List {
    Node* head;
} List;

int* get_first_elements(List** lists, int num_lists, int* out_count) {
    if (lists == NULL || num_lists <= 0 || out_count == NULL) {
        return NULL;
    }

    int* result = (int*)malloc((size_t)num_lists * sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < num_lists; i++) {
        if (lists[i] != NULL && lists[i]->head != NULL) {
            result[count++] = lists[i]->head->data;
        }
    }

    *out_count = count;
    if (count == 0) {
        free(result);
        return NULL;
    }

    int* final_result = (int*)realloc(result, (size_t)count * sizeof(int));
    if (final_result == NULL) {
        free(result);
        return NULL;
    }

    return final_result;
}

Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

List* create_list(int* arr, int size) {
    if (arr == NULL || size <= 0) {
        return NULL;
    }

    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    Node* current = NULL;

    for (int i = 0; i < size; i++) {
        Node* new_node = create_node(arr[i]);
        if (new_node == NULL) {
            while (list->head != NULL) {
                Node* temp = list->head;
                list->head = list->head->next;
                free(temp);
            }
            free(list);
            return NULL;
        }

        if (list->head == NULL) {
            list->head = new_node;
            current = new_node;
        } else {
            current->next = new_node;
            current = new_node;
        }
    }

    return list;
}

void free_list(List* list) {
    if (list == NULL) {
        return;
    }

    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

int main(void) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5};
    int arr3[] = {6, 7, 8, 9};

    List* list1 = create_list(arr1, 3);
    List* list2 = create_list(arr2, 2);
    List* list3 = create_list(arr3, 4);

    if (list1 == NULL || list2 == NULL || list3 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_list(list1);
        free_list(list2);
        free_list(list3);
        return 1;
    }

    List* lists[] = {list1, list2, list3};
    int count = 0;
    int* first_elements = get_first_elements(lists, 3, &count);

    if (first_elements == NULL) {
        fprintf(stderr, "Failed to get first elements\n");
        free_list(list1);
        free_list(list2);
        free_list(list3);
        return 1;
    }

    printf("First elements: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", first_elements[i]);
    }
    printf("\n");

    free(first_elements);
    free_list(list1);
    free_list(list2);
    free_list(list3);

    return 0;
}