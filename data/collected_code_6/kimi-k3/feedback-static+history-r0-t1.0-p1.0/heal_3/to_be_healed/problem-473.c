#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int *data;
    size_t size;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
} TupleList;

static int compare_ints(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) return -1;
    if (ia > ib) return 1;
    return 0;
}

static int *copy_sorted(const int *data, size_t size) {
    int *copy;
    if (size == 0) return NULL;
    if (size > SIZE_MAX / sizeof(int)) return NULL;
    copy = malloc(size * sizeof(int));
    if (copy == NULL) return NULL;
    /* Possible weaknesses found:
     *  implicit declaration of function 'memcpy_s'; did you mean 'memcpy'? [-Wimplicit-function-declaration]
     *  did you mean 'memcpy'?
     *  call to undeclared function 'memcpy_s'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
     */
    if (memcpy_s(copy, size * sizeof(int), data, size * sizeof(int)) != 0) {
        free(copy);
        return NULL;
    }
    qsort(copy, size, sizeof(int), compare_ints);
    return copy;
}

static int *sorted_intersection(const int *a, size_t a_size,
                                const int *b, size_t b_size,
                                size_t *out_size) {
    size_t i = 0, j = 0, k = 0;
    size_t max_size = (a_size < b_size) ? a_size : b_size;
    int *result;

    *out_size = 0;
    if (max_size == 0) return NULL;

    if (max_size > SIZE_MAX / sizeof(int)) return NULL;
    result = malloc(max_size * sizeof(int));
    if (result == NULL) return NULL;

    while (i < a_size && j < b_size) {
        if (a[i] < b[j]) {
            i++;
        } else if (a[i] > b[j]) {
            j++;
        } else {
            if (k == 0 || result[k - 1] != a[i]) {
                result[k++] = a[i];
            }
            i++;
            j++;
        }
    }

    *out_size = k;
    return result;
}

int *tuple_intersection(const TupleList *list, size_t *result_size) {
    int *current;
    int *next;
    size_t current_size;
    size_t next_size;
    size_t i;

    *result_size = 0;
 /* Possible weaknesses found:
  *  'memcpy' declared here
  */

    if (list == NULL || list->tuples == NULL || list->count == 0) {
        return NULL;
    }

    for (i = 0; i < list->count; i++) {
        if (list->tuples[i].size > 0 && list->tuples[i].data == NULL) {
            return NULL;
        }
    }

    current = copy_sorted(list->tuples[0].data, list->tuples[0].size);
    if (current == NULL && list->tuples[0].size > 0) {
        return NULL;
    }
    current_size = list->tuples[0].size;

    if (current != NULL) {
        size_t unique_count = 0;
        for (i = 0; i < current_size; i++) {
            if (unique_count == 0 || current[unique_count - 1] != current[i]) {
                current[unique_count++] = current[i];
            }
        }
        current_size = unique_count;
    }

    for (i = 1; i < list->count; i++) {
        int *sorted_tuple = copy_sorted(list->tuples[i].data,
                                        list->tuples[i].size);
        if (sorted_tuple == NULL && list->tuples[i].size > 0) {
            free(current);
            return NULL;
        }

        next = sorted_intersection(current, current_size,
                                   sorted_tuple, list->tuples[i].size,
                                   &next_size);
        free(sorted_tuple);
        free(current);

        if (next == NULL && next_size == 0) {
            current = NULL;
            current_size = 0;
            break;
        }

        current = next;
        current_size = next_size;
    }

    *result_size = current_size;
    return current;
}

static void free_tuple_list(TupleList *list) {
    size_t i;
    if (list == NULL) return;
    for (i = 0; i < list->count; i++) {
        free(list->tuples[i].data);
    }
    free(list->tuples);
    list->tuples = NULL;
    list->count = 0;
}

int main(void) {
    TupleList list;
    int *result;
    size_t result_size;
    size_t i;
    int status = EXIT_SUCCESS;

    int data1[] = {3, 1, 4, 1, 5, 9, 2, 6};
    int data2[] = {5, 3, 5, 8, 9, 7, 9};
    int data3[] = {2, 7, 1, 8, 2, 8, 1, 5, 3};

    list.count = 3;
    list.tuples = malloc(list.count * sizeof(Tuple));
    if (list.tuples == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    list.tuples[0].size = sizeof(data1) / sizeof(data1[0]);
    list.tuples[0].data = malloc(list.tuples[0].size * sizeof(int));
    list.tuples[1].size = sizeof(data2) / sizeof(data2[0]);
    list.tuples[1].data = malloc(list.tuples[1].size * sizeof(int));
    list.tuples[2].size = sizeof(data3) / sizeof(data3[0]);
    list.tuples[2].data = malloc(list.tuples[2].size * sizeof(int));

    if (list.tuples[0].data == NULL ||
        list.tuples[1].data == NULL ||
        list.tuples[2].data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    if (list.tuples[0].size > SIZE_MAX / sizeof(int)) {
        fprintf(stderr, "Size overflow detected\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }
    /* Possible weaknesses found:
     *  call to undeclared function 'memcpy_s'; ISO C99 and later do not support implicit function declarations [-Wimplicit-function-declaration]
     */
    if (memcpy_s(list.tuples[0].data, list.tuples[0].size * sizeof(int),
                 data1, list.tuples[0].size * sizeof(int)) != 0) {
        fprintf(stderr, "Memory copy failed\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    if (list.tuples[1].size > SIZE_MAX / sizeof(int)) {
        fprintf(stderr, "Size overflow detected\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }
    if (memcpy_s(list.tuples[1].data, list.tuples[1].size * sizeof(int),
                 data2, list.tuples[1].size * sizeof(int)) != 0) {
        fprintf(stderr, "Memory copy failed\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    if (list.tuples[2].size > SIZE_MAX / sizeof(int)) {
        fprintf(stderr, "Size overflow detected\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }
    if (memcpy_s(list.tuples[2].data, list.tuples[2].size * sizeof(int),
                 data3, list.tuples[2].size * sizeof(int)) != 0) {
        fprintf(stderr, "Memory copy failed\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    result = tuple_intersection(&list, &result_size);

    if (result == NULL && result_size == 0 && list.count > 0) {
        int all_valid = 1;
        for (i = 0; i < list.count; i++) {
            if (list.tuples[i].size > 0 && list.tuples[i].data == NULL) {
                all_valid = 0;
            }
        }
        if (!all_valid) {
            fprintf(stderr, "Error computing intersection\n");
            status = EXIT_FAILURE;
        }
    }

    if (status == EXIT_SUCCESS) {
        printf("Intersection: ");
        if (result_size == 0) {
            printf("(empty)");
        } else {
            for (i = 0; i < result_size; i++) {
                printf("%d%s", result[i],
                       (i + 1 < result_size) ? ", " : "");
            }
        }
        printf("\n");
    }

    free(result);
    free_tuple_list(&list);

    return status;
}