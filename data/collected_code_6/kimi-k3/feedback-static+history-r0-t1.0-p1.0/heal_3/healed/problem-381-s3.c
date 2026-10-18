#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
} InnerList;

typedef struct {
    InnerList *lists;
    size_t count;
} ListOfLists;

int compare_by_index(const void *a, const void *b, void *context) {
    size_t index = *(size_t *)context;
    const InnerList *list_a = (const InnerList *)a;
    const InnerList *list_b = (const InnerList *)b;

    if (list_a->size <= index && list_b->size <= index) {
        return 0;
    }
    if (list_a->size <= index) {
        return 1;
    }
    if (list_b->size <= index) {
        return -1;
    }

    if (list_a->data[index] < list_b->data[index]) {
        return -1;
    }
    if (list_a->data[index] > list_b->data[index]) {
        return 1;
    }
    return 0;
}

int sort_lists_by_index(ListOfLists *list_of_lists, size_t index) {
    if (list_of_lists == NULL || list_of_lists->lists == NULL) {
        return -1;
    }
    if (list_of_lists->count == 0) {
        return 0;
    }

    qsort_r(list_of_lists->lists, list_of_lists->count, sizeof(InnerList),
            (int (*)(const void *, const void *, void *))compare_by_index, &index);
    return 0;
}

void free_list_of_lists(ListOfLists *list_of_lists) {
    if (list_of_lists == NULL) {
        return;
    }
    if (list_of_lists->lists != NULL) {
        for (size_t i = 0; i < list_of_lists->count; i++) {
            free(list_of_lists->lists[i].data);
            list_of_lists->lists[i].data = NULL;
        }
        free(list_of_lists->lists);
        list_of_lists->lists = NULL;
    }
    list_of_lists->count = 0;
}

int main(void) {
    ListOfLists list_of_lists = {NULL, 0};
    size_t num_lists = 4;
    size_t sort_index = 1;

    list_of_lists.lists = malloc(num_lists * sizeof(InnerList));
    if (list_of_lists.lists == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    list_of_lists.count = num_lists;

    int values[][3] = {
        {5, 2, 9},
        {1, 8, 3},
        {4, 1, 7},
        {3, 5, 2}
    };

    for (size_t i = 0; i < num_lists; i++) {
        list_of_lists.lists[i].size = 3;
        list_of_lists.lists[i].data = malloc(3 * sizeof(int));
        if (list_of_lists.lists[i].data == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            list_of_lists.count = i;
            free_list_of_lists(&list_of_lists);
            return EXIT_FAILURE;
        }
        for (size_t j = 0; j < 3; j++) {
            list_of_lists.lists[i].data[j] = values[i][j];
        }
    }

    printf("Before sorting:\n");
    for (size_t i = 0; i < list_of_lists.count; i++) {
        printf("List %zu: ", i);
        for (size_t j = 0; j < list_of_lists.lists[i].size; j++) {
            printf("%d ", list_of_lists.lists[i].data[j]);
        }
        printf("\n");
    }

    if (sort_lists_by_index(&list_of_lists, sort_index) != 0) {
        fprintf(stderr, "Sorting failed\n");
        free_list_of_lists(&list_of_lists);
        return EXIT_FAILURE;
    }

    printf("\nAfter sorting by index %zu:\n", sort_index);
    for (size_t i = 0; i < list_of_lists.count; i++) {
        printf("List %zu: ", i);
        for (size_t j = 0; j < list_of_lists.lists[i].size; j++) {
            printf("%d ", list_of_lists.lists[i].data[j]);
        }
        printf("\n");
    }

    free_list_of_lists(&list_of_lists);
    return EXIT_SUCCESS;
}