#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int *data;
    int size;
    size_t capacity;
} IntList;

typedef struct {
    IntList *lists;
    int count;
    int capacity;
} ListOfLists;

int init_list_of_lists(ListOfLists *lol, int capacity) {
    if (lol == NULL || capacity <= 0) {
        return -1;
    }
    lol->lists = (IntList *)calloc((size_t)capacity, sizeof(IntList));
    if (lol->lists == NULL) {
        return -1;
    }
    lol->count = 0;
    lol->capacity = capacity;
    return 0;
}

int add_list(ListOfLists *lol, const int *data, int size) {
    size_t bytes;
    int *new_data;
    if (lol == NULL || lol->lists == NULL || data == NULL || size <= 0) {
        return -1;
    }
    if (lol->count < 0 || lol->capacity <= 0 || lol->count >= lol->capacity) {
        return -1;
    }
    if ((size_t)size > SIZE_MAX / sizeof(int)) {
        return -1;
    }
    bytes = (size_t)size * sizeof(int);
    if (bytes == 0 || bytes / sizeof(int) != (size_t)size) {
        return -1;
    }
    new_data = (int *)malloc(bytes);
    if (new_data == NULL) {
        return -1;
    }
    if ((size_t)size > bytes / sizeof(int)) {
        free(new_data);
        return -1;
    }
    memcpy(new_data, data, bytes);
    lol->lists[lol->count].data = new_data;
    lol->lists[lol->count].size = size;
    lol->lists[lol->count].capacity = bytes;
    lol->count++;
    return 0;
}

int compare_lists(const IntList *a, const IntList *b) {
    int i;
    if (a == NULL || b == NULL) {
        return 0;
    }
    if (a->data == NULL || b->data == NULL) {
        return 0;
    }
    if (a->size < 0 || a->size != b->size) {
        return 0;
    }
    for (i = 0; i < a->size; i++) {
        if (a->data[i] != b->data[i]) {
            return 0;
        }
    }
    return 1;
}

int remove_duplicate_lists(ListOfLists *lol) {
    int i, j, k;
    if (lol == NULL || lol->lists == NULL) {
        return -1;
    }
    for (i = 0; i < lol->count; i++) {
        for (j = i + 1; j < lol->count; j++) {
            if (compare_lists(&lol->lists[i], &lol->lists[j])) {
                free(lol->lists[j].data);
                for (k = j; k < lol->count - 1; k++) {
                    lol->lists[k] = lol->lists[k + 1];
                }
                lol->lists[lol->count - 1].data = NULL;
                lol->lists[lol->count - 1].size = 0;
                lol->lists[lol->count - 1].capacity = 0;
                lol->count--;
                j--;
            }
        }
    }
    return 0;
}

void free_list_of_lists(ListOfLists *lol) {
    int i;
    if (lol == NULL) {
        return;
    }
    if (lol->lists != NULL) {
        for (i = 0; i < lol->count; i++) {
            free(lol->lists[i].data);
            lol->lists[i].data = NULL;
            lol->lists[i].size = 0;
            lol->lists[i].capacity = 0;
        }
        free(lol->lists);
        lol->lists = NULL;
    }
    lol->count = 0;
    lol->capacity = 0;
}

void print_list_of_lists(const ListOfLists *lol) {
    int i, j;
    if (lol == NULL || lol->lists == NULL) {
        return;
    }
    for (i = 0; i < lol->count; i++) {
        printf("[");
        for (j = 0; j < lol->lists[i].size; j++) {
            printf("%d", lol->lists[i].data[j]);
            if (j < lol->lists[i].size - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

int main(void) {
    ListOfLists lol;
    const int list1[] = {1, 2, 3};
    const int list2[] = {4, 5, 6};
    const int list3[] = {1, 2, 3};
    const int list4[] = {7, 8, 9};
    const int list5[] = {4, 5, 6};
    const int list6[] = {1, 2, 3};

    if (init_list_of_lists(&lol, 10) != 0) {
        fprintf(stderr, "Failed to initialize list of lists\n");
        return EXIT_FAILURE;
    }

    if (add_list(&lol, list1, 3) != 0 ||
        add_list(&lol, list2, 3) != 0 ||
        add_list(&lol, list3, 3) != 0 ||
        add_list(&lol, list4, 3) != 0 ||
        add_list(&lol, list5, 3) != 0 ||
        add_list(&lol, list6, 3) != 0) {
        fprintf(stderr, "Failed to add list\n");
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }

    printf("Before removing duplicates:\n");
    print_list_of_lists(&lol);

    if (remove_duplicate_lists(&lol) != 0) {
        fprintf(stderr, "Failed to remove duplicates\n");
        free_list_of_lists(&lol);
        return EXIT_FAILURE;
    }

    printf("\nAfter removing duplicates:\n");
    print_list_of_lists(&lol);

    free_list_of_lists(&lol);
    return EXIT_SUCCESS;
}