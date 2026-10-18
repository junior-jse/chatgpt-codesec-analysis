#include <stdio.h>
#include <stdlib.h>

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
    int x = *(const int *)a;
    int y = *(const int *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static int tuple_contains(const Tuple *t, int value)
{
    size_t i;
    for (i = 0; i < t->size; i++) {
        if (t->data[i] == value) {
            return 1;
        }
    }
    return 0;
}

Tuple *tuple_intersection(const TupleList *list)
{
    Tuple *result;
    size_t i, j, k;
    int value;
    int exists_in_all;

    if (list == NULL || list->tuples == NULL || list->count == 0) {
        return NULL;
    }

    result = malloc(sizeof(Tuple));
    if (result == NULL) {
        return NULL;
    }

    result->data = malloc(list->tuples[0].size * sizeof(int));
    if (result->data == NULL && list->tuples[0].size > 0) {
        free(result);
        return NULL;
    }

    result->size = 0;

    for (i = 0; i < list->tuples[0].size; i++) {
        value = list->tuples[0].data[i];
        exists_in_all = 1;

        for (j = 1; j < list->count; j++) {
            if (!tuple_contains(&list->tuples[j], value)) {
                exists_in_all = 0;
                break;
            }
        }

        if (exists_in_all) {
            int already_added = 0;
            for (k = 0; k < result->size; k++) {
                if (result->data[k] == value) {
                    already_added = 1;
                    break;
                }
            }
            if (!already_added) {
                result->data[result->size] = value;
                result->size++;
            }
        }
    }

    if (result->size == 0) {
        free(result->data);
        result->data = NULL;
    }

    return result;
}

void free_tuple(Tuple *t)
{
    if (t != NULL) {
        free(t->data);
        free(t);
    }
}

int main(void)
{
    int data1[] = {3, 1, 4, 1, 5};
    int data2[] = {4, 3, 2, 1};
    int data3[] = {3, 4, 6, 1};

    Tuple t1 = {data1, 5};
    Tuple t2 = {data2, 4};
    Tuple t3 = {data3, 4};

    Tuple tuples[] = {t1, t2, t3};
    TupleList list = {tuples, 3};

    Tuple *result = tuple_intersection(&list);

    if (result != NULL) {
        size_t i;
        printf("Intersection: ");
        for (i = 0; i < result->size; i++) {
            printf("%d ", result->data[i]);
        }
        printf("\n");
        free_tuple(result);
    } else {
        printf("No intersection or error occurred.\n");
    }

    return 0;
}