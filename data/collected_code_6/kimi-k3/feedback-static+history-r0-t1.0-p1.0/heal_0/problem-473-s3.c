#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
} TupleList;

static int compare_ints(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

static int contains(const int *arr, size_t n, int val)
{
    size_t i;
    for (i = 0; i < n; i++) {
        if (arr[i] == val) return 1;
    }
    return 0;
}

Tuple tuple_intersection(const TupleList *list)
{
    Tuple result = {NULL, 0};
    size_t i, j, k;
    int *tmp = NULL;
    size_t cap;

    if (list == NULL || list->count == 0 || list->tuples == NULL) {
        return result;
    }

    cap = list->tuples[0].size;
    if (cap == 0) {
        return result;
    }

    tmp = (int *)malloc(cap * sizeof(int));
    if (tmp == NULL) {
        return result;
    }

    for (i = 0; i < list->tuples[0].size; i++) {
        int val = list->tuples[0].data[i];
        int found_in_all = 1;

        if (contains(tmp, result.size, val)) {
            continue;
        }

        for (j = 1; j < list->count; j++) {
            int found = 0;
            for (k = 0; k < list->tuples[j].size; k++) {
                if (list->tuples[j].data[k] == val) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                found_in_all = 0;
                break;
            }
        }

        if (found_in_all) {
            tmp[result.size++] = val;
        }
    }

    if (result.size > 0) {
        int *final_data = (int *)malloc(result.size * sizeof(int));
        if (final_data == NULL) {
            free(tmp);
            result.size = 0;
            return result;
        }
        memcpy(final_data, tmp, result.size * sizeof(int));
        result.data = final_data;
    }

    free(tmp);
    return result;
}

void free_tuple(Tuple *t)
{
    if (t != NULL) {
        free(t->data);
        t->data = NULL;
        t->size = 0;
    }
}

void free_tuple_list(TupleList *list)
{
    size_t i;
    if (list != NULL) {
        for (i = 0; i < list->count; i++) {
            free(list->tuples[i].data);
        }
        free(list->tuples);
        list->tuples = NULL;
        list->count = 0;
    }
}

int main(void)
{
    TupleList list;
    Tuple result;
    size_t i;

    list.count = 3;
    list.tuples = (Tuple *)malloc(list.count * sizeof(Tuple));
    if (list.tuples == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    list.tuples[0].size = 4;
    list.tuples[0].data = (int *)malloc(list.tuples[0].size * sizeof(int));
    if (list.tuples[0].data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(list.tuples);
        return EXIT_FAILURE;
    }
    list.tuples[0].data[0] = 3;
    list.tuples[0].data[1] = 1;
    list.tuples[0].data[2] = 4;
    list.tuples[0].data[3] = 2;

    list.tuples[1].size = 3;
    list.tuples[1].data = (int *)malloc(list.tuples[1].size * sizeof(int));
    if (list.tuples[1].data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(list.tuples[0].data);
        free(list.tuples);
        return EXIT_FAILURE;
    }
    list.tuples[1].data[0] = 1;
    list.tuples[1].data[1] = 4;
    list.tuples[1].data[2] = 5;

    list.tuples[2].size = 4;
    list.tuples[2].data = (int *)malloc(list.tuples[2].size * sizeof(int));
    if (list.tuples[2].data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(list.tuples[1].data);
        free(list.tuples[0].data);
        free(list.tuples);
        return EXIT_FAILURE;
    }
    list.tuples[2].data[0] = 4;
    list.tuples[2].data[1] = 1;
    list.tuples[2].data[2] = 7;
    list.tuples[2].data[3] = 9;

    result = tuple_intersection(&list);

    printf("Intersection: ");
    for (i = 0; i < result.size; i++) {
        printf("%d ", result.data[i]);
    }
    printf("\n");

    free_tuple(&result);
    free_tuple_list(&list);

    return EXIT_SUCCESS;
}