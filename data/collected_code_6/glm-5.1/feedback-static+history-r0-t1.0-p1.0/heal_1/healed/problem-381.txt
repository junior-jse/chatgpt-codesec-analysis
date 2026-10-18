#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
} InnerList;

typedef struct {
    InnerList *lists;
    size_t size;
} OuterList;

int compare_inner_lists(const void *a, const void *b) {
    const InnerList *list_a = (const InnerList *)a;
    const InnerList *list_b = (const InnerList *)b;

    if (list_a->size == 0 && list_b->size == 0) {
        return 0;
    }
    if (list_a->size == 0) {
        return -1;
    }
    if (list_b->size == 0) {
        return 1;
    }

    if (list_a->data[0] < list_b->data[0]) {
        return -1;
    }
    if (list_a->data[0] > list_b->data[0]) {
        return 1;
    }

    return 0;
}

OuterList create_outer_list(size_t outer_size, const size_t *inner_sizes, int **values) {
    OuterList outer;
    outer.size = outer_size;
    outer.lists = (InnerList *)malloc(outer_size * sizeof(InnerList));

    for (size_t i = 0; i < outer_size; i++) {
        outer.lists[i].size = inner_sizes[i];
        outer.lists[i].data = (int *)malloc(inner_sizes[i] * sizeof(int));
        for (size_t j = 0; j < inner_sizes[i]; j++) {
            outer.lists[i].data[j] = values[i][j];
        }
    }

    return outer;
}

void free_outer_list(OuterList *outer) {
    for (size_t i = 0; i < outer->size; i++) {
        free(outer->lists[i].data);
    }
    free(outer->lists);
    outer->lists = NULL;
    outer->size = 0;
}

void sort_list_of_lists(OuterList *outer, size_t index) {
    if (outer == NULL || outer->size == 0) {
        return;
    }

    for (size_t i = 0; i < outer->size; i++) {
        if (outer->lists[i].size > 0 && index < outer->lists[i].size) {
            int temp = outer->lists[i].data[index];
            outer->lists[i].data[index] = outer->lists[i].data[0];
            outer->lists[i].data[0] = temp;
        }
    }

    qsort(outer->lists, outer->size, sizeof(InnerList), compare_inner_lists);

    for (size_t i = 0; i < outer->size; i++) {
        if (outer->lists[i].size > 0 && index < outer->lists[i].size) {
            int temp = outer->lists[i].data[0];
            outer->lists[i].data[0] = outer->lists[i].data[index];
            outer->lists[i].data[index] = temp;
        }
    }
}

void print_outer_list(const OuterList *outer) {
    for (size_t i = 0; i < outer->size; i++) {
        printf("[");
        for (size_t j = 0; j < outer->lists[i].size; j++) {
            printf("%d", outer->lists[i].data[j]);
            if (j < outer->lists[i].size - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

int main(void) {
    const size_t inner_sizes[] = {3, 2, 4, 1};
    int val0[] = {9, 2, 7};
    int val1[] = {4, 5};
    int val2[] = {1, 8, 3, 6};
    int val3[] = {10};
    int *values[] = {val0, val1, val2, val3};

    OuterList outer = create_outer_list(4, inner_sizes, values);

    printf("Before sorting:\n");
    print_outer_list(&outer);

    sort_list_of_lists(&outer, 1);

    printf("\nAfter sorting by index 1:\n");
    print_outer_list(&outer);

    free_outer_list(&outer);

    return 0;
}