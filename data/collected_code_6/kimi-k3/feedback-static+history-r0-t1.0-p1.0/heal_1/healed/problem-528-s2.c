#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    size_t length;
} LinkedList;

typedef struct {
    LinkedList *lists;
    size_t count;
    size_t capacity;
} ListCollection;

typedef struct {
    size_t *indices;
    size_t count;
    size_t capacity;
} MinIndices;

Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

int append_node(LinkedList *list, int data) {
    Node *node;
    Node *current;

    if (list == NULL) {
        return -1;
    }

    node = create_node(data);
    if (node == NULL) {
        return -1;
    }

    if (list->head == NULL) {
        list->head = node;
    } else {
        current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }
    list->length++;
    return 0;
}

void free_list(LinkedList *list) {
    Node *current;
    Node *next;

    if (list == NULL) {
        return;
    }

    current = list->head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->length = 0;
}

int init_collection(ListCollection *collection, size_t capacity) {
    if (collection == NULL || capacity == 0) {
        return -1;
    }

    collection->lists = (LinkedList *)calloc(capacity, sizeof(LinkedList));
    if (collection->lists == NULL) {
        return -1;
    }
    collection->count = 0;
    collection->capacity = capacity;
    return 0;
}

int add_list_to_collection(ListCollection *collection, const LinkedList *list) {
    if (collection == NULL || list == NULL) {
        return -1;
    }
    if (collection->count >= collection->capacity) {
        return -1;
    }

    collection->lists[collection->count] = *list;
    collection->count++;
    return 0;
}

void free_collection(ListCollection *collection) {
    if (collection == NULL) {
        return;
    }

    if (collection->lists != NULL) {
        for (size_t i = 0; i < collection->count; i++) {
            free_list(&collection->lists[i]);
        }
        free(collection->lists);
        collection->lists = NULL;
    }
    collection->count = 0;
    collection->capacity = 0;
}

int find_min_length_lists(const ListCollection *collection, MinIndices *result) {
    size_t min_length;
    size_t new_capacity;
    size_t *temp;

    if (collection == NULL || result == NULL || collection->count == 0) {
        return -1;
    }

    result->indices = NULL;
    result->count = 0;
    result->capacity = 0;

    min_length = collection->lists[0].length;
    for (size_t i = 1; i < collection->count; i++) {
        if (collection->lists[i].length < min_length) {
            min_length = collection->lists[i].length;
        }
    }

    for (size_t i = 0; i < collection->count; i++) {
        if (collection->lists[i].length == min_length) {
            if (result->count >= result->capacity) {
                new_capacity = (result->capacity == 0) ? 4 : result->capacity * 2;
                temp = (size_t *)realloc(result->indices, new_capacity * sizeof(size_t));
                if (temp == NULL) {
                    free(result->indices);
                    result->indices = NULL;
                    result->count = 0;
                    result->capacity = 0;
                    return -1;
                }
                result->indices = temp;
                result->capacity = new_capacity;
            }
            result->indices[result->count] = i;
            result->count++;
        }
    }

    return 0;
}

void free_min_indices(MinIndices *result) {
    if (result == NULL) {
        return;
    }
    if (result->indices != NULL) {
        free(result->indices);
        result->indices = NULL;
    }
    result->count = 0;
    result->capacity = 0;
}

void print_list(const LinkedList *list) {
    Node *current;

    if (list == NULL) {
        return;
    }

    current = list->head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf(" (length: %zu)\n", list->length);
}

int main(void) {
    ListCollection collection;
    MinIndices result;
    LinkedList list1 = {NULL, 0};
    LinkedList list2 = {NULL, 0};
    LinkedList list3 = {NULL, 0};
    LinkedList list4 = {NULL, 0};
    int status = EXIT_SUCCESS;

    if (init_collection(&collection, 4) != 0) {
        fprintf(stderr, "Failed to initialize collection\n");
        return EXIT_FAILURE;
    }

    if (append_node(&list1, 1) != 0 || append_node(&list1, 2) != 0 ||
        append_node(&list1, 3) != 0) {
        fprintf(stderr, "Failed to build list1\n");
        status = EXIT_FAILURE;
        goto cleanup_lists;
    }

    if (append_node(&list2, 10) != 0) {
        fprintf(stderr, "Failed to build list2\n");
        status = EXIT_FAILURE;
        goto cleanup_lists;
    }

    if (append_node(&list3, 20) != 0 || append_node(&list3, 30) != 0) {
        fprintf(stderr, "Failed to build list3\n");
        status = EXIT_FAILURE;
        goto cleanup_lists;
    }

    if (append_node(&list4, 40) != 0) {
        fprintf(stderr, "Failed to build list4\n");
        status = EXIT_FAILURE;
        goto cleanup_lists;
    }

    if (add_list_to_collection(&collection, &list1) != 0 ||
        add_list_to_collection(&collection, &list2) != 0 ||
        add_list_to_collection(&collection, &list3) != 0 ||
        add_list_to_collection(&collection, &list4) != 0) {
        fprintf(stderr, "Failed to add lists to collection\n");
        status = EXIT_FAILURE;
        goto cleanup_lists;
    }

    list1.head = NULL;
    list2.head = NULL;
    list3.head = NULL;
    list4.head = NULL;

    if (find_min_length_lists(&collection, &result) != 0) {
        fprintf(stderr, "Failed to find minimum length lists\n");
        status = EXIT_FAILURE;
        goto cleanup_collection;
    }

    printf("Lists with minimum length:\n");
    for (size_t i = 0; i < result.count; i++) {
        printf("List %zu: ", result.indices[i]);
        print_list(&collection.lists[result.indices[i]]);
    }

    free_min_indices(&result);

cleanup_collection:
    free_collection(&collection);
    return status;

cleanup_lists:
    free_list(&list1);
    free_list(&list2);
    free_list(&list3);
    free_list(&list4);
    free_collection(&collection);
    return status;
}