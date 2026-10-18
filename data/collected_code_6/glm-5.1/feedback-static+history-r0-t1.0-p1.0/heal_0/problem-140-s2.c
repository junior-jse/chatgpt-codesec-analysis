#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

int* extract_singly(Tuple* tuples, int size, int* result_size) {
    int* elements = (int*)malloc(size * 2 * sizeof(int));
    if (!elements) {
        *result_size = 0;
        return NULL;
    }

    int elem_count = 0;
    for (int i = 0; i < size; i++) {
        elements[elem_count++] = tuples[i].first;
        elements[elem_count++] = tuples[i].second;
    }

    int* counts = (int*)calloc(elem_count, sizeof(int));
    if (!counts) {
        free(elements);
        *result_size = 0;
        return NULL;
    }

    for (int i = 0; i < elem_count; i++) {
        for (int j = 0; j < elem_count; j++) {
            if (elements[i] == elements[j]) {
                counts[i]++;
            }
        }
    }

    int* result = (int*)malloc(elem_count * sizeof(int));
    if (!result) {
        free(elements);
        free(counts);
        *result_size = 0;
        return NULL;
    }

    int res_idx = 0;
    for (int i = 0; i < elem_count; i++) {
        if (counts[i] == 1) {
            result[res_idx++] = elements[i];
        }
    }

    free(elements);
    free(counts);

    *result_size = res_idx;
    return result;
}

int main() {
    Tuple tuples[] = {{1, 2}, {3, 4}, {2, 5}, {6, 4}};
    int size = sizeof(tuples) / sizeof(tuples[0]);
    int result_size = 0;

    int* singly_elements = extract_singly(tuples, size, &result_size);

    if (singly_elements != NULL) {
        for (int i = 0; i < result_size; i++) {
            printf("%d ", singly_elements[i]);
        }
        printf("\n");
        free(singly_elements);
    }

    return 0;
}