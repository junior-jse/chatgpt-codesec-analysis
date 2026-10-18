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
} ListOfLists;

static int compare_lists(const void *a, const void *b) {
    const IntList *la = (const IntList *)a;
    const IntList *lb = (const IntList *)b;
    if (la->size != lb->size) {
        return (la->size < lb->size) ? -1 : 1;
    }
    return memcmp(la->data, lb->data, la->size * sizeof(int));
}

static int are_lists_equal(const IntList *a, const IntList *b) {
    if (a->size != b->size) {
        return 0;
    }
    if (a->size == 0) {
        return 1;
    }
    return memcmp(a->data, b->data, a->size * sizeof(int)) == 0;
}

static void free_list(IntList *list) {
    if (list != NULL) {
        free(list->data);
        list->data = NULL;
        list->size = 0;
    }
}

static void free_list_of_lists(ListOfLists *lol) {
    if (lol != NULL) {
        for (size_t i = 0; i < lol->count; i++) {
            free_list(&lol->lists[i]);
        }
        free(lol->lists);
        lol->lists = NULL;
        lol->count = 0;
    }
}

int remove_duplicate_lists(ListOfLists *lol) {
    if (lol == NULL) {
        return -1;
    }
    if (lol->count <= 1) {
        return 0;
    }

    qsort(lol->lists, lol->count, sizeof(IntList), compare_lists);

    size_t write_idx = 0;
    for (size_t read_idx = 1; read_idx < lol->count; read_idx++) {
        if (!are_lists_equal(&lol->lists[write_idx], &lol->lists[read_idx])) {
            write_idx++;
            if (write_idx != read_idx) {
                free_list(&lol->lists[write_idx]);
                lol->lists[write_idx] = lol->lists[read_idx];
                lol->lists[read_idx].data = NULL;
                lol->lists[read_idx].size = 0;
            }
        } else {
            free_list(&lol->lists[read_idx]);
        }
    }

    lol->count = write_idx + 1;
    return 0;
}

static IntList create_list(const int *arr, size_t size) {
    IntList list;
    list.size = size;
    if (size == 0) {
        list.data = NULL;
        return list;
    }
    list.data = malloc(size * sizeof(int));
    if (list.data == NULL) {
        list.size = 0;
        return list;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(list.data, arr, size * sizeof(int));
    return list;
}

static void print_list_of_lists(const ListOfLists *lol) {
    if (lol == NULL) {
        return;
    }
    printf("[");
    for (size_t i = 0; i < lol->count; i++) {
        printf("[");
        for (size_t j = 0; j < lol->lists[i].size; j++) {
            printf("%d", lol->lists[i].data[j]);
            if (j < lol->lists[i].size - 1) {
                printf(", ");
            }
        }
        printf("]");
        if (i < lol->count - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(void) {
    /* Possible weaknesses found:
     *  Variable 'arr1' can be declared as const array [constVariable]
     */
    int arr1[] = {1, 2, 3};
    /* Possible weaknesses found:
     *  Variable 'arr2' can be declared as const array [constVariable]
     */
    int arr2[] = {4, 5};
    /* Possible weaknesses found:
     *  Variable 'arr3' can be declared as const array [constVariable]
     */
    int arr3[] = {1, 2, 3};
    /* Possible weaknesses found:
     *  Variable 'arr4' can be declared as const array [constVariable]
     */
    int arr4[] = {6, 7, 8};
    /* Possible weaknesses found:
     *  Variable 'arr5' can be declared as const array [constVariable]
     */
    int arr5[] = {4, 5};
    /* Possible weaknesses found:
     *  Variable 'arr6' can be declared as const array [constVariable]
     */
    int arr6[] = {1, 2};

    ListOfLists lol;
    lol.count = 6;
    lol.lists = malloc(lol.count * sizeof(IntList));
    if (lol.lists == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    lol.lists[0] = create_list(arr1, 3);
    lol.lists[1] = create_list(arr2, 2);
    lol.lists[2] = create_list(arr3, 3);
    lol.lists[3] = create_list(arr4, 3);
    lol.lists[4] = create_list(arr5, 2);
    lol.lists[5] = create_list(arr6, 2);

    for (size_t i = 0; i < lol.count; i++) {
        if (lol.lists[i].data == NULL && lol.lists[i].size > 0) {
            fprintf(stderr, "Memory allocation failed\n");
            free_list_of_lists(&lol);
            return EXIT_FAILURE;
        }
    }

    printf("Before: ");
    print_list_of_lists(&lol);

    if (remove_duplicate_lists(&lol) != 0) {
        fprintf(stderr, "Error removing duplicates\n");
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }

    printf("After: ");
    print_list_of_lists(&lol);

    free_list_of_lists(&lol);
    return EXIT_SUCCESS;
}