#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct List {
    int *data;
    size_t size;
} List;

bool list_equal(const List *a, const List *b) {
    if (a == NULL || b == NULL) {
        return false;
    }
    if (a->size != b->size) {
        return false;
    }
    for (size_t i = 0; i < a->size; i++) {
        if (a->data[i] != b->data[i]) {
            return false;
        }
    }
    return true;
}

List remove_duplicates(List *lists, size_t size, size_t *out_size) {
    /* Possible weaknesses found:
     *  Assuming that condition 'out_size==NULL' is not redundant
     */
    if (lists == NULL || out_size == NULL || size == 0) {
        /* Possible weaknesses found:
         *  Either the condition 'out_size==NULL' is redundant or there is possible null pointer dereference: out_size. [nullPointerRedundantCheck]
         *  Null pointer dereference
         */
        *out_size = 0;
        List empty = {NULL, 0};
        return empty;
    }

    bool *keep = (bool *)malloc(size * sizeof(bool));
    if (keep == NULL) {
        *out_size = 0;
        List empty = {NULL, 0};
        return empty;
    }

    for (size_t i = 0; i < size; i++) {
        keep[i] = true;
    }

    for (size_t i = 0; i < size; i++) {
        if (!keep[i]) {
            continue;
        }
        for (size_t j = i + 1; j < size; j++) {
            if (keep[j] && list_equal(&lists[i], &lists[j])) {
                keep[j] = false;
            }
        }
    }

    size_t new_size = 0;
    for (size_t i = 0; i < size; i++) {
        if (keep[i]) {
            new_size++;
        }
    }

    List result;
    result.data = NULL;
    result.size = new_size;

    if (new_size > 0) {
        result.data = (int *)malloc(new_size * sizeof(int));
        if (result.data == NULL) {
            free(keep);
            *out_size = 0;
            List empty = {NULL, 0};
            return empty;
        }

        size_t index = 0;
        for (size_t i = 0; i < size; i++) {
            if (keep[i]) {
                result.data[index] = lists[i].data[0];
                index++;
            }
        }
    }

    free(keep);
    *out_size = new_size;
    return result;
}

int main() {
    size_t num_lists = 5;
    List *lists = (List *)malloc(num_lists * sizeof(List));
    if (lists == NULL) {
        return EXIT_FAILURE;
    }

    int a1[] = {1, 2};
    int a2[] = {1, 2};
    int a3[] = {3, 4, 5};
    int a4[] = {1, 2};
    int a5[] = {3, 4, 5};

    lists[0].data = a1; lists[0].size = 2;
    lists[1].data = a2; lists[1].size = 2;
    lists[2].data = a3; lists[2].size = 3;
    lists[3].data = a4; lists[3].size = 2;
    lists[4].data = a5; lists[4].size = 3;

    size_t out_size = 0;
    List unique_lists = remove_duplicates(lists, num_lists, &out_size);

    if (unique_lists.data != NULL) {
        for (size_t i = 0; i < out_size; i++) {
            printf("%d ", unique_lists.data[i]);
        }
        printf("\n");
        free(unique_lists.data);
    }

    free(lists);

    return EXIT_SUCCESS;
}