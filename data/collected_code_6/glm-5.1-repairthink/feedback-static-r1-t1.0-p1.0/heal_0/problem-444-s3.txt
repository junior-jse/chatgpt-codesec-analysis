#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *elements;
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
        return result;
    }

    for (int i = 0; i < list->count; i++) {
        if (list->tuples[i].size < k) {
            result.tuples[i].size = 0;
            result.tuples[i].elements = NULL;
        } else {
            int new_size = list->tuples[i].size - 2 * k;
            if (new_size <= 0) {
                result.tuples[i].size = 0;
                result.tuples[i].elements = NULL;
            } else {
                result.tuples[i].size = new_size;
                result.tuples[i].elements = (int *)malloc(new_size * sizeof(int));
                if (result.tuples[i].elements == NULL) {
                    for (int j = 0; j < i; j++) {
                        free(result.tuples[j].elements);
                    }
                    free(result.tuples);
                    result.tuples = NULL;
                    result.count = 0;
                    return result;
                }
                for (int j = 0; j < new_size; j++) {
                    result.tuples[i].elements[j] = list->tuples[i].elements[j + k];
                }
            }
        }
    }
    return result;
}

void free_tuple_list(TupleList *list) {
    if (list != NULL) {
        for (int i = 0; i < list->count; i++) {
            free(list->tuples[i].elements);
        }
        free(list->tuples);
        list->count = 0;
        list->tuples = NULL;
    }
}

int main(void) {
    TupleList list;
    list.count = 2;
    list.tuples = (Tuple *)malloc(2 * sizeof(Tuple));
    
    list.tuples[0].size = 5;
    list.tuples[0].elements = (int *)malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++) list.tuples[0].elements[i] = i + 1;

    list.tuples[1].size = 6;
    list.tuples[1].elements = (int *)malloc(6 * sizeof(int));
    for (int i = 0; i < 6; i++) list.tuples[1].elements[i] = (i + 1) * 10;

    int k = 1;
    TupleList trimmed = trim_tuples(&list, k);

    for (int i = 0; i < trimmed.count; i++) {
        for (int j = 0; j < trimmed.tuples[i].size; j++) {
            printf("%d ", trimmed.tuples[i].elements[j]);
        }
        printf("\n");
    }

    free_tuple_list(&list);
    free_tuple_list(&trimmed);

    return 0;
}