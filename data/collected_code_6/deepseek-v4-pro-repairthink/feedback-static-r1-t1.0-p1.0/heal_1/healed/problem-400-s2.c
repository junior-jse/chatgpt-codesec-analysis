#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int **tuples;
    int *counts;
    int size;
    int capacity;
    int tuple_len;
} TupleFrequency;

static int tuple_equal(const int *a, const int *b, int len) {
    for (int i = 0; i < len; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

static void copy_tuple(int *dest, const int *src, int len) {
    for (int i = 0; i < len; i++) {
        dest[i] = src[i];
    }
}

static void sort_tuple(int *tuple, int len) {
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (tuple[i] > tuple[j]) {
                int temp = tuple[i];
                tuple[i] = tuple[j];
                tuple[j] = temp;
            }
        }
    }
}

static void init_frequency(TupleFrequency *tf, int tuple_len) {
    tf->capacity = 16;
    tf->size = 0;
    tf->tuple_len = tuple_len;
    tf->tuples = (int **)malloc(tf->capacity * sizeof(int *));
    tf->counts = (int *)malloc(tf->capacity * sizeof(int));
    if (!tf->tuples || !tf->counts) {
        free(tf->tuples);
        free(tf->counts);
        tf->tuples = NULL;
        tf->counts = NULL;
        tf->capacity = 0;
        return;
    }
}

static int add_or_update(TupleFrequency *tf, const int *tuple) {
    if (!tf || !tf->tuples || !tf->counts) {
        return -1;
    }

    int *sorted = (int *)malloc(tf->tuple_len * sizeof(int));
    if (!sorted) {
        return -1;
    }
    copy_tuple(sorted, tuple, tf->tuple_len);
    sort_tuple(sorted, tf->tuple_len);

    for (int i = 0; i < tf->size; i++) {
        if (tuple_equal(tf->tuples[i], sorted, tf->tuple_len)) {
            tf->counts[i]++;
            free(sorted);
            return 0;
        }
    }

    if (tf->size >= tf->capacity) {
        tf->capacity *= 2;
        int **new_tuples = (int **)realloc(tf->tuples, tf->capacity * sizeof(int *));
        if (!new_tuples) {
            free(sorted);
            return -1;
        }
        tf->tuples = new_tuples;
        int *new_counts = (int *)realloc(tf->counts, tf->capacity * sizeof(int));
        if (!new_counts) {
            free(sorted);
            return -1;
        }
        tf->counts = new_counts;
    }

    int *new_tuple = (int *)malloc(tf->tuple_len * sizeof(int));
    if (!new_tuple) {
        free(sorted);
        return -1;
    }
    copy_tuple(new_tuple, sorted, tf->tuple_len);
    free(sorted);

    tf->tuples[tf->size] = new_tuple;
    tf->counts[tf->size] = 1;
    tf->size++;
    return 0;
}

static void free_frequency(TupleFrequency *tf) {
    if (tf) {
        if (tf->tuples) {
            for (int i = 0; i < tf->size; i++) {
                free(tf->tuples[i]);
            }
            free(tf->tuples);
        }
        free(tf->counts);
        tf->tuples = NULL;
        tf->counts = NULL;
        tf->size = 0;
        tf->capacity = 0;
    }
}

int main(void) {
    int test_data[][3] = {
        {1, 2, 3},
        {2, 1, 3},
        {3, 2, 1},
        {4, 5, 6},
        {6, 5, 4},
        {7, 8, 9}
    };
    int num_tuples = sizeof(test_data) / sizeof(test_data[0]);
    int tuple_len = 3;
    TupleFrequency tf;

    init_frequency(&tf, tuple_len);
    if (!tf.tuples || !tf.counts) {
        fprintf(stderr, "Failed to initialize\n");
        return 1;
    }

    for (int i = 0; i < num_tuples; i++) {
        if (add_or_update(&tf, test_data[i]) != 0) {
            fprintf(stderr, "Failed to process tuple\n");
            free_frequency(&tf);
            return 1;
        }
    }

    printf("Tuple frequencies (order independent):\n");
    for (int i = 0; i < tf.size; i++) {
        printf("(");
        for (int j = 0; j < tf.tuple_len; j++) {
            printf("%d", tf.tuples[i][j]);
            if (j < tf.tuple_len - 1) {
                printf(",");
            }
        }
        printf(") -> %d\n", tf.counts[i]);
    }

    free_frequency(&tf);
    return 0;
}