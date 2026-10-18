#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    int len;
} List;

int compare_ints(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

int list_equal(const void *a, const void *b) {
    const List *la = (const List *)a;
    const List *lb = (const List *)b;
    if (la->len != lb->len) {
        return 0;
    }
    for (int i = 0; i < la->len; i++) {
        if (la->data[i] != lb->data[i]) {
            return 0;
        }
    }
    return 1;
}

int count_unique_lists(List *lists, int count) {
    if (lists == NULL || count <= 0) {
        return 0;
    }

    List *sorted_lists = (List *)malloc(count * sizeof(List));
    if (sorted_lists == NULL) {
        return 0;
    }

    for (int i = 0; i < count; i++) {
        sorted_lists[i].len = lists[i].len;
        if (lists[i].len > 0) {
            sorted_lists[i].data = (int *)malloc(lists[i].len * sizeof(int));
            if (sorted_lists[i].data == NULL) {
                for (int j = 0; j < i; j++) {
                    free(sorted_lists[j].data);
                }
                free(sorted_lists);
                return 0;
            }
            memcpy(sorted_lists[i].data, lists[i].data, lists[i].len * sizeof(int));
            qsort(sorted_lists[i].data, sorted_lists[i].len, sizeof(int), compare_ints);
        } else {
            sorted_lists[i].data = NULL;
        }
    }

    int unique_count = 0;
    int *is_unique = (int *)malloc(count * sizeof(int));
    if (is_unique == NULL) {
        for (int i = 0; i < count; i++) {
            free(sorted_lists[i].data);
        }
        free(sorted_lists);
        return 0;
    }

    for (int i = 0; i < count; i++) {
        is_unique[i] = 1;
    }

    for (int i = 0; i < count; i++) {
        if (is_unique[i]) {
            unique_count++;
            for (int j = i + 1; j < count; j++) {
                if (is_unique[j] && list_equal(&sorted_lists[i], &sorted_lists[j])) {
                    is_unique[j] = 0;
                }
            }
        }
    }

    free(is_unique);
    for (int i = 0; i < count; i++) {
        free(sorted_lists[i].data);
    }
    free(sorted_lists);

    return unique_count;
}

int main(void) {
    int a1[] = {1, 2, 3};
    int a2[] = {3, 2, 1};
    int a3[] = {4, 5};
    int a4[] = {1, 2, 3, 4};
    int a5[] = {5, 4};
    int a6[] = {7};
    int a7[] = {7};

    (void)lists;

    int num_lists = 7;
    printf("%zu\n", (size_t)num_lists);
    
    int *safe_a1 = (int *)malloc(3 * sizeof(int));
    int *safe_a2 = (int *)malloc(3 * sizeof(int));
    int *safe_a3 = (int *)malloc(2 * sizeof(int));
    int *safe_a4 = (int *)malloc(4 * sizeof(int));
    int *safe_a5 = (int *)malloc(2 * sizeof(int));
    int *safe_a6 = (int *)malloc(1 * sizeof(int));
    int *safe_a7 = (int *)malloc(1 * sizeof(int));

    memcpy(safe_a1, a1, 3 * sizeof(int));
    memcpy(safe_a2, a2, 3 * sizeof(int));
    memcpy(safe_a3, a3, 2 * sizeof(int));
    memcpy(safe_a4, a4, 4 * sizeof(int));
    memcpy(safe_a5, a5, 2 * sizeof(int));
    memcpy(safe_a6, a6, 1 * sizeof(int));
    memcpy(safe_a7, a7, 1 * sizeof(int));

    List safe_lists[] = {
        {safe_a1, 3},
        {safe_a2, 3},
        {safe_a3, 2},
        {safe_a4, 4},
        {safe_a5, 2},
        {safe_a6, 1},
        {safe_a7, 1}
    };

    int unique = count_unique_lists(safe_lists, num_lists);
    printf("%d\n", unique);

    free(safe_a1);
    free(safe_a2);
    free(safe_a3);
    free(safe_a4);
    free(safe_a5);
    free(safe_a6);
    free(safe_a7);

    return 0;
}