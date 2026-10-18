#include <stdio.h>
#include <stdlib.h>

/* Possible weaknesses found:
 *  Parameter 'sizes' can be declared as pointer to const [constParameterPointer]
 */
int** reverse_lists(int** lists, int* sizes, int count) {
    if (lists == NULL || sizes == NULL || count <= 0) {
        return NULL;
    }

    int** reversed = malloc(count * sizeof(int*));
    if (reversed == NULL) {
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        int sz = sizes[i];
        reversed[i] = malloc(sz * sizeof(int));
        if (reversed[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(reversed[j]);
            }
            free(reversed);
            return NULL;
        }
        for (int j = 0; j < sz; j++) {
            reversed[i][j] = lists[i][sz - 1 - j];
        }
    }

    return reversed;
}

void free_lists(int** lists, int count) {
    if (lists == NULL) {
        return;
    }
    for (int i = 0; i < count; i++) {
        free(lists[i]);
    }
    free(lists);
}

int main() {
    int list1[] = {1, 2, 3};
    int list2[] = {4, 5};
    int list3[] = {6, 7, 8, 9};

    int count = 3;
    int* lists[] = {list1, list2, list3};
    int sizes[] = {3, 2, 4};

    int** reversed = reverse_lists(lists, sizes, count);
    if (reversed == NULL) {
        return 1;
    }

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < sizes[i]; j++) {
            printf("%d ", reversed[i][j]);
        }
        printf("\n");
    }

    free_lists(reversed, count);

    return 0;
}