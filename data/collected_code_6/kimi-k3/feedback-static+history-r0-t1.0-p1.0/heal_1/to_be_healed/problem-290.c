#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t length;
} IntList;

typedef struct {
    IntList *lists;
    size_t count;
    size_t capacity;
} ListCollection;

int init_collection(ListCollection *collection, size_t initial_capacity) {
    if (collection == NULL || initial_capacity == 0) {
        return 0;
    }
    collection->lists = malloc(initial_capacity * sizeof(IntList));
    if (collection->lists == NULL) {
        return 0;
    }
    collection->count = 0;
    collection->capacity = initial_capacity;
    return 1;
}

int add_list(ListCollection *collection, const int *data, size_t length) {
    if (collection == NULL || (data == NULL && length > 0)) {
        return 0;
    }
    if (collection->count == collection->capacity) {
        size_t new_capacity = collection->capacity * 2;
        IntList *new_lists = realloc(collection->lists, new_capacity * sizeof(IntList));
        if (new_lists == NULL) {
            return 0;
        }
        collection->lists = new_lists;
        collection->capacity = new_capacity;
    }
    collection->lists[collection->count].data = malloc(length * sizeof(int));
    if (collection->lists[collection->count].data == NULL && length > 0) {
        return 0;
    }
    if (length > 0) {
        for (size_t i = 0; i < length; i++) {
            collection->lists[collection->count].data[i] = data[i];
        }
    }
    collection->lists[collection->count].length = length;
    collection->count++;
    return 1;
}

size_t find_max_length(const ListCollection *collection) {
    if (collection == NULL || collection->count == 0) {
        return 0;
    }
    size_t max_length = collection->lists[0].length;
    for (size_t i = 1; i < collection->count; i++) {
        if (collection->lists[i].length > max_length) {
            max_length = collection->lists[i].length;
        }
    }
    return max_length;
}

int find_lists_with_max_length(const ListCollection *collection, size_t **indices, size_t *result_count) {
    if (collection == NULL || indices == NULL || result_count == NULL) {
        return 0;
    }
    *indices = NULL;
    *result_count = 0;
    if (collection->count == 0) {
        return 1;
    }
    size_t max_length = find_max_length(collection);
    size_t count = 0;
    for (size_t i = 0; i < collection->count; i++) {
        if (collection->lists[i].length == max_length) {
            count++;
        }
    }
    size_t *temp_indices = malloc(count * sizeof(size_t));
    if (temp_indices == NULL) {
        return 0;
    }
    size_t idx = 0;
    for (size_t i = 0; i < collection->count; i++) {
        if (collection->lists[i].length == max_length) {
            temp_indices[idx++] = i;
        }
    }
    *indices = temp_indices;
    *result_count = count;
    return 1;
}

void free_collection(ListCollection *collection) {
    if (collection == NULL) {
        return;
    }
    for (size_t i = 0; i < collection->count; i++) {
        free(collection->lists[i].data);
    }
    free(collection->lists);
    collection->lists = NULL;
    collection->count = 0;
    collection->capacity = 0;
}

int main(void) {
    ListCollection collection;
    if (!init_collection(&collection, 4)) {
        fprintf(stderr, "Failed to initialize collection\n");
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     *  Variable 'data1' can be declared as const array [constVariable]
     */
    int data1[] = {1, 2, 3};
    /* Possible weaknesses found:
     *  Variable 'data2' can be declared as const array [constVariable]
     */
    int data2[] = {4, 5};
    /* Possible weaknesses found:
     *  Variable 'data3' can be declared as const array [constVariable]
     */
    int data3[] = {6, 7, 8, 9};
    /* Possible weaknesses found:
     *  Variable 'data4' can be declared as const array [constVariable]
     */
    int data4[] = {10, 11, 12, 13};
    /* Possible weaknesses found:
     *  Variable 'data5' can be declared as const array [constVariable]
     */
    int data5[] = {14};

    if (!add_list(&collection, data1, 3) ||
        !add_list(&collection, data2, 2) ||
        !add_list(&collection, data3, 4) ||
        !add_list(&collection, data4, 4) ||
        !add_list(&collection, data5, 1)) {
        fprintf(stderr, "Failed to add list\n");
        free_collection(&collection);
        return EXIT_FAILURE;
    }

    size_t *indices = NULL;
    size_t result_count = 0;
    if (!find_lists_with_max_length(&collection, &indices, &result_count)) {
        fprintf(stderr, "Failed to find lists with maximum length\n");
        free_collection(&collection);
        return EXIT_FAILURE;
    }

    printf("Lists with maximum length (%zu):\n", find_max_length(&collection));
    for (size_t i = 0; i < result_count; i++) {
        size_t list_idx = indices[i];
        printf("List %zu: [", list_idx);
        for (size_t j = 0; j < collection.lists[list_idx].length; j++) {
            printf("%d", collection.lists[list_idx].data[j]);
            if (j < collection.lists[list_idx].length - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }

    free(indices);
    free_collection(&collection);
    return EXIT_SUCCESS;
}