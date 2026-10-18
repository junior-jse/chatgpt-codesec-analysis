#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
} Tuple;

typedef struct {
    Tuple *tuples;
    int count;
} TupleList;

TupleList trim_tuples(const TupleList *list, int k) {
    TupleList result;
    result.count = list->count;
    result.tuples = (Tuple *)malloc(result.count * sizeof(Tuple));

    if (result.tuples == NULL) {
        result.count = 0;
        result.tuples = NULL;
        return result;
    }

    for (int i = 0; i < list->count; ++i) {
        int original_size = list->tuples[i].size;
        int new_size = original_size - 2 * k;

        if (new_size <= 0) {
            result.tuples[i].data = NULL;
            result.tuples[i].size = 0;
        } else {
            result.tuples[i].data = (int *)malloc(new_size * sizeof(int));
            if (result.tuples[i].data == NULL) {
                for (int j = 0; j <= i; ++j) {
                    free(result.tuples[j].data);
                }
                free(result.tuples);
                result.tuples = NULL;
                result.count = 0;
                return result;
            }
            for (int j = 0; j < new_size; ++j) {
                result.tuples[i].data[j] = list->tuples[i].data[k + j];
            }
            result.tuples[i].size = new_size;
        }
    }

    return result;
}

void free_tuple_list(TupleList *list) {
    if (list->tuples != NULL) {
        for (int i = 0; i < list->count; ++i) {
            free(list->tuples[i].data);
        }
        free(list->tuples);
        list->tuples = NULL;
    }
    list->count = 0;
}

int main() {
    int data1[] = {1, 2, 3, 4, 5};
    int data2[] = {10, 20, 30, 40, 50, 60};
    int data3[] = {7, 8, 9};

    Tuple original_tuples[] = {
        {data1, 5},
        {data2, 6},
        {data3, 3}
    };

    TupleList list = {original_tuples, 3};

    int k = 1;
    TupleList trimmed = trim_tuples(&list, k);

    if (trimmed.tuples != NULL) {
        for (int i = 0; i < trimmed.count; ++i) {
            printf("Tuple %d (size %d): ", i, trimmed.tuples[i].size);
            for (int j = 0; j < trimmed.tuples[i].size; ++j) {
                printf("%d ", trimmed.tuples[i].data[j]);
            }
            printf("\n");
        }
    }

    free_tuple_list(&trimmed);

    return 0;
}