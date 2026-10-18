#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *elements;
    int size;
} Tuple;

typedef struct {
    Tuple *tuples;
    int count;
} TupleList;

int compare_tuples(const void *a, const void *b) {
    const Tuple *ta = (const Tuple *)a;
    const Tuple *tb = (const Tuple *)b;
    if (ta->size == 0 || tb->size == 0)
        return 0;
    return ta->elements[0] - tb->elements[0];
}

TupleList join_tuples(Tuple *tuples, int tuple_count) {
    TupleList result;
    result.tuples = NULL;
    result.count = 0;

    if (tuples == NULL || tuple_count <= 0) {
        return result;
    }

    qsort(tuples, tuple_count, sizeof(Tuple), compare_tuples);

    result.tuples = malloc(sizeof(Tuple) * tuple_count);
    if (result.tuples == NULL) {
        return result;
    }

    int result_count = 0;
    int i = 0;

    while (i < tuple_count) {
        int j = i;
        int total_size = 0;
        while (j < tuple_count && tuples[i].elements[0] == tuples[j].elements[0]) {
            total_size += tuples[j].size;
            j++;
        }

        result.tuples[result_count].elements = malloc(sizeof(int) * total_size);
        if (result.tuples[result_count].elements == NULL) {
            for (int k = 0; k < result_count; k++) {
                free(result.tuples[k].elements);
            }
            free(result.tuples);
            result.tuples = NULL;
            result.count = 0;
            return result;
        }

        result.tuples[result_count].size = total_size;
        int pos = 0;
        for (int k = i; k < j; k++) {
            memcpy(result.tuples[result_count].elements + pos, tuples[k].elements,
                   sizeof(int) * tuples[k].size);
            pos += tuples[k].size;
        }

        result_count++;
        i = j;
    }

    result.count = result_count;
    return result;
}

void free_tuple_list(TupleList *list) {
    if (list == NULL || list->tuples == NULL) {
        return;
    }
    for (int i = 0; i < list->count; i++) {
        free(list->tuples[i].elements);
    }
    free(list->tuples);
    list->tuples = NULL;
    list->count = 0;
}

int main(void) {
    Tuple t1 = {(int[]){1, 2, 3}, 3};
    Tuple t2 = {(int[]){1, 4}, 2};
    Tuple t3 = {(int[]){2, 5, 6, 7}, 4};
    Tuple t4 = {(int[]){2, 8}, 2};
    Tuple t5 = {(int[]){3, 9}, 2};

    Tuple tuples[] = {t1, t2, t3, t4, t5};
    int tuple_count = sizeof(tuples) / sizeof(tuples[0]);

    TupleList result = join_tuples(tuples, tuple_count);

    if (result.tuples == NULL && tuple_count > 0) {
        return 1;
    }

    for (int i = 0; i < result.count; i++) {
        printf("(");
        for (int j = 0; j < result.tuples[i].size; j++) {
            printf("%d", result.tuples[i].elements[j]);
            if (j < result.tuples[i].size - 1) {
                printf(", ");
            }
        }
        printf(")\n");
    }

    free_tuple_list(&result);
    return 0;
}