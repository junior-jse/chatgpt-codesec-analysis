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

int compare_ints(const void *a, const void *b) {
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

int *tuple_intersection(TupleList *list, int *result_size) {
    int *result = NULL;
    int capacity = 0;
    int count = 0;

    /* Possible weaknesses found:
     *  Assuming that condition 'result_size==NULL' is not redundant
     */
    if (list == NULL || list->count <= 0 || result_size == NULL) {
        /* Possible weaknesses found:
         *  Either the condition 'result_size==NULL' is redundant or there is possible null pointer dereference: result_size. [nullPointerRedundantCheck]
         *  Null pointer dereference
         */
        *result_size = 0;
        return NULL;
    }

    for (int i = 0; i < list->tuples[0].size; i++) {
        int candidate = list->tuples[0].elements[i];
        int in_all = 1;

        for (int j = 1; j < list->count; j++) {
            int found = 0;
            for (int k = 0; k < list->tuples[j].size; k++) {
                if (list->tuples[j].elements[k] == candidate) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                in_all = 0;
                break;
            }
        }

        if (in_all) {
            int already_added = 0;
            for (int m = 0; m < count; m++) {
                if (result[m] == candidate) {
                    already_added = 1;
                    break;
                }
            }

            if (!already_added) {
                if (count >= capacity) {
                    capacity = (capacity == 0) ? 4 : capacity * 2;
                    int *new_result = (int *)realloc(result, capacity * sizeof(int));
                    if (new_result == NULL) {
                        free(result);
                        *result_size = 0;
                        return NULL;
                    }
                    result = new_result;
                }
                result[count++] = candidate;
            }
        }
    }

    if (result != NULL) {
        qsort(result, count, sizeof(int), compare_ints);
    }

    *result_size = count;
    return result;
}

int main(void) {
    int tuple1_data[] = {3, 1, 2, 4, 5};
    int tuple2_data[] = {2, 4, 6, 1};
    int tuple3_data[] = {4, 1, 7, 2};

    Tuple tuple1 = {tuple1_data, 5};
    Tuple tuple2 = {tuple2_data, 4};
    Tuple tuple3 = {tuple3_data, 4};

    Tuple tuples[] = {tuple1, tuple2, tuple3};
    TupleList list = {tuples, 3};

    int result_size = 0;
    int *result = tuple_intersection(&list, &result_size);

    if (result != NULL) {
        for (int i = 0; i < result_size; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    } else {
        printf("No intersection found\n");
    }

    return 0;
}