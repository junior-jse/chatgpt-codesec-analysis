#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* elements;
    int size;
    int capacity;
} Set;

Set* create_set(int capacity) {
    Set* s = (Set*)malloc(sizeof(Set));
    if (s == NULL) return NULL;
    s->elements = (int*)malloc(capacity * sizeof(int));
    if (s->elements == NULL) {
        free(s);
        return NULL;
    }
    s->size = 0;
    s->capacity = capacity;
    return s;
}

void free_set(Set* s) {
    if (s != NULL) {
        free(s->elements);
        free(s);
    }
}

int contains(const Set* s, int value) {
    for (int i = 0; i < s->size; i++) {
        if (s->elements[i] == value) return 1;
    }
    return 0;
}

int insert(Set* s, int value) {
    if (s == NULL || s->size == s->capacity) return 0;
    if (contains(s, value)) return 1;
    s->elements[s->size++] = value;
    return 1;
}

Set* tuples_to_set(const int* tuples, int num_tuples, int tuple_size) {
    if (tuples == NULL || num_tuples <= 0 || tuple_size <= 0) return NULL;
    int total_elements = num_tuples * tuple_size;
    Set* s = create_set(total_elements);
    if (s == NULL) return NULL;
    for (int i = 0; i < num_tuples; i++) {
        for (int j = 0; j < tuple_size; j++) {
            int idx = i * tuple_size + j;
            if (!insert(s, tuples[idx])) {
                free_set(s);
                return NULL;
            }
        }
    }
    return s;
}

int main() {
    int tuples[][3] = {
        {1, 2, 3},
        {2, 3, 4},
        {4, 5, 1}
    };
    int num_tuples = sizeof(tuples) / sizeof(tuples[0]);
    int tuple_size = sizeof(tuples[0]) / sizeof(tuples[0][0]);

    Set* s = tuples_to_set((const int*)tuples, num_tuples, tuple_size);
    if (s == NULL) {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < s->size; i++) {
        printf("%d ", s->elements[i]);
    }
    printf("\n");

    free_set(s);
    return EXIT_SUCCESS;
}