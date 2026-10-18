#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

int* extract_singly(Tuple* tuples, size_t size, size_t* out_size) {
    int* elements = NULL;
    size_t capacity = 0;
    *out_size = 0;

    if (size == 0) {
        return NULL;
    }

    elements = (int*)malloc(size * 2 * sizeof(int));
    if (!elements) {
        *out_size = 0;
        return NULL;
    }
    capacity = size * 2;

    for (size_t i = 0; i < size; ++i) {
        elements[(*out_size)++] = tuples[i].first;
        if (tuples[i].first != tuples[i].second) {
            elements[(*out_size)++] = tuples[i].second;
        }
    }

    int* singly = (int*)malloc(*out_size * sizeof(int));
    if (!singly) {
        free(elements);
        *out_size = 0;
        return NULL;
    }

    size_t sing_size = 0;
    for (size_t i = 0; i < *out_size; ++i) {
        size_t count = 0;
        for (size_t j = 0; j < *out_size; ++j) {
            if (elements[i] == elements[j]) {
                ++count;
            }
        }
        if (count == 1) {
            singly[sing_size++] = elements[i];
        }
    }

    free(elements);

    if (sing_size == 0) {
        free(singly);
        *out_size = 0;
        return NULL;
    }

    int* final_singly = (int*)realloc(singly, sing_size * sizeof(int));
    if (final_singly) {
        *out_size = sing_size;
        return final_singly;
    }

    *out_size = sing_size;
    return singly;
}

int main(void) {
    Tuple tuples[] = {{1, 2}, {3, 4}, {2, 5}, {6, 1}};
    size_t size = sizeof(tuples) / sizeof(tuples[0]);
    size_t out_size = 0;

    int* result = extract_singly(tuples, size, &out_size);

    if (result) {
        for (size_t i = 0; i < out_size; ++i) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    }

    return 0;
}