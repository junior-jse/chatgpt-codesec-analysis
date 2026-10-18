#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
} IntList;

typedef struct {
    IntList *lists;
    size_t count;
    size_t capacity;
} ListCollection;

static int compare_int_lists(const IntList *a, const IntList *b)
{
    size_t i;
    size_t min_size;

    if (a == NULL || b == NULL) {
        return (a == b) ? 0 : ((a == NULL) ? -1 : 1);
    }

    if (a->size != b->size) {
        return (a->size < b->size) ? -1 : 1;
    }

    min_size = a->size;
    for (i = 0; i < min_size; ++i) {
        if (a->data[i] != b->data[i]) {
            return (a->data[i] < b->data[i]) ? -1 : 1;
        }
    }

    return 0;
}

static int int_list_equal(const IntList *a, const IntList *b)
{
    return compare_int_lists(a, b) == 0;
}

static int init_list_collection(ListCollection *collection, size_t initial_capacity)
{
    if (collection == NULL || initial_capacity == 0) {
        return -1;
    }

    collection->lists = malloc(initial_capacity * sizeof(IntList));
    if (collection->lists == NULL) {
        return -1;
    }

    collection->count = 0;
    collection->capacity = initial_capacity;
    return 0;
}

static void free_int_list(IntList *list)
{
    if (list != NULL) {
        free(list->data);
        list->data = NULL;
        list->size = 0;
    }
}

static void free_list_collection(ListCollection *collection)
{
    size_t i;

    if (collection == NULL) {
        return;
    }

    if (collection->lists != NULL) {
        for (i = 0; i < collection->count; ++i) {
            free_int_list(&collection->lists[i]);
        }
        free(collection->lists);
    }

    collection->lists = NULL;
    collection->count = 0;
    collection->capacity = 0;
}

static int add_int_list(ListCollection *collection, const int *data, size_t size)
{
    IntList *new_lists;
    IntList new_list;
    size_t new_capacity;

    if (collection == NULL || (data == NULL && size > 0)) {
        return -1;
    }

    if (collection->count >= collection->capacity) {
        new_capacity = collection->capacity * 2;
        if (new_capacity < collection->capacity) {
            return -1;
        }

        new_lists = realloc(collection->lists, new_capacity * sizeof(IntList));
        if (new_lists == NULL) {
            return -1;
        }

        collection->lists = new_lists;
        collection->capacity = new_capacity;
    }

    new_list.data = NULL;
    new_list.size = size;

    if (size > 0) {
        new_list.data = malloc(size * sizeof(int));
        if (new_list.data == NULL) {
            return -1;
        }
        memcpy(new_list.data, data, size * sizeof(int));
    }

    collection->lists[collection->count] = new_list;
    collection->count++;

    return 0;
}

static size_t count_unique_lists(const ListCollection *collection)
{
    size_t unique_count = 0;
    size_t i;
    size_t j;
    int is_duplicate;

    if (collection == NULL || collection->lists == NULL) {
        return 0;
    }

    for (i = 0; i < collection->count; ++i) {
        is_duplicate = 0;

        for (j = 0; j < i; ++j) {
            if (int_list_equal(&collection->lists[i], &collection->lists[j])) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            unique_count++;
        }
    }

    return unique_count;
}

int main(void)
{
    ListCollection collection;
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5};
    int data3[] = {1, 2, 3};
    int data4[] = {6, 7, 8, 9};
    int data5[] = {4, 5};
    int data6[] = {1, 2, 4};
    size_t unique_count;

    if (init_list_collection(&collection, 4) != 0) {
        fprintf(stderr, "Failed to initialize collection\n");
        return EXIT_FAILURE;
    }

    if (add_int_list(&collection, data1, 3) != 0) {
        fprintf(stderr, "Failed to add list\n");
        free_list_collection(&collection);
        return EXIT_FAILURE;
    }

    if (add_int_list(&collection, data2, 2) != 0) {
        fprintf(stderr, "Failed to add list\n");
        free_list_collection(&collection);
        return EXIT_FAILURE;
    }

    if (add_int_list(&collection, data3, 3) != 0) {
        fprintf(stderr, "Failed to add list\n");
        free_list_collection(&collection);
        return EXIT_FAILURE;
    }

    if (add_int_list(&collection, data4, 4) != 0) {
        fprintf(stderr, "Failed to add list\n");
        free_list_collection(&collection);
        return EXIT_FAILURE;
    }

    if (add_int_list(&collection, data5, 2) != 0) {
        fprintf(stderr, "Failed to add list\n");
        free_list_collection(&collection);
        return EXIT_FAILURE;
    }

    if (add_int_list(&collection, data6, 3) != 0) {
        fprintf(stderr, "Failed to add list\n");
        free_list_collection(&collection);
        return EXIT_FAILURE;
    }

    unique_count = count_unique_lists(&collection);

    printf("Total lists: %zu\n", collection.count);
    printf("Unique lists: %zu\n", unique_count);

    free_list_collection(&collection);

    return EXIT_SUCCESS;
}