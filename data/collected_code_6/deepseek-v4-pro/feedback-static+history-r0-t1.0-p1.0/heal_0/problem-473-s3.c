#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *elements;
    int size;
} Tuple;

typedef struct {
    Tuple *tuples;
    int size;
} TupleList;

typedef struct {
    int *elements;
    int size;
    int capacity;
} IntArray;

static int compare_ints(const void *a, const void *b) {
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    return (int_a > int_b) - (int_a < int_b);
}

static IntArray *int_array_create(int capacity) {
    IntArray *arr = (IntArray *)malloc(sizeof(IntArray));
    if (arr == NULL) {
        return NULL;
    }
    arr->elements = (int *)malloc(sizeof(int) * capacity);
    if (arr->elements == NULL) {
        free(arr);
        return NULL;
    }
    arr->size = 0;
    arr->capacity = capacity;
    return arr;
}

static void int_array_destroy(IntArray *arr) {
    if (arr != NULL) {
        free(arr->elements);
        free(arr);
    }
}

static int int_array_add(IntArray *arr, int value) {
    if (arr->size >= arr->capacity) {
        int new_capacity = arr->capacity * 2;
        int *new_elements = (int *)realloc(arr->elements, sizeof(int) * new_capacity);
        if (new_elements == NULL) {
            return 0;
        }
        arr->elements = new_elements;
        arr->capacity = new_capacity;
    }
    arr->elements[arr->size++] = value;
    return 1;
}

static int int_array_contains(const IntArray *arr, int value) {
    for (int i = 0; i < arr->size; i++) {
        if (arr->elements[i] == value) {
            return 1;
        }
    }
    return 0;
}

static int *tuple_to_sorted_unique_array(const Tuple *tuple, int *out_size) {
    if (tuple == NULL || tuple->size <= 0 || tuple->elements == NULL) {
        *out_size = 0;
        return NULL;
    }

    int *sorted = (int *)malloc(sizeof(int) * tuple->size);
    if (sorted == NULL) {
        *out_size = 0;
        return NULL;
    }

    memcpy(sorted, tuple->elements, sizeof(int) * tuple->size);
    qsort(sorted, tuple->size, sizeof(int), compare_ints);

    int unique_count = 1;
    for (int i = 1; i < tuple->size; i++) {
        if (sorted[i] != sorted[i - 1]) {
            unique_count++;
        }
    }

    int *unique = (int *)malloc(sizeof(int) * unique_count);
    if (unique == NULL) {
        free(sorted);
        *out_size = 0;
        return NULL;
    }

    int unique_idx = 0;
    unique[unique_idx++] = sorted[0];
    for (int i = 1; i < tuple->size; i++) {
        if (sorted[i] != sorted[i - 1]) {
            unique[unique_idx++] = sorted[i];
        }
    }

    free(sorted);
    *out_size = unique_idx;
    return unique;
}

Tuple tuple_intersection(const TupleList *list) {
    Tuple result;
    result.elements = NULL;
    result.size = 0;

    if (list == NULL || list->size <= 0 || list->tuples == NULL) {
        return result;
    }

    IntArray *intersection = int_array_create(16);
    if (intersection == NULL) {
        return result;
    }

    int first_size = 0;
    int *first_unique = tuple_to_sorted_unique_array(&list->tuples[0], &first_size);
    if (first_unique == NULL) {
        int_array_destroy(intersection);
        return result;
    }

    for (int i = 0; i < first_size; i++) {
        int candidate = first_unique[i];
        int found_in_all = 1;

        for (int j = 1; j < list->size; j++) {
            int current_size = 0;
            int *current_unique = tuple_to_sorted_unique_array(&list->tuples[j], &current_size);
            if (current_unique == NULL) {
                found_in_all = 0;
                break;
            }

            int found = 0;
            for (int k = 0; k < current_size; k++) {
                if (current_unique[k] == candidate) {
                    found = 1;
                    break;
                }
            }

            free(current_unique);
            if (!found) {
                found_in_all = 0;
                break;
            }
        }

        if (found_in_all) {
            if (!int_array_add(intersection, candidate)) {
                free(first_unique);
                int_array_destroy(intersection);
                return result;
            }
        }
    }

    free(first_unique);

    if (intersection->size > 0) {
        result.elements = (int *)malloc(sizeof(int) * intersection->size);
        if (result.elements == NULL) {
            int_array_destroy(intersection);
            return result;
        }
        memcpy(result.elements, intersection->elements, sizeof(int) * intersection->size);
        result.size = intersection->size;
        qsort(result.elements, result.size, sizeof(int), compare_ints);
    }

    int_array_destroy(intersection);
    return result;
}

int main(void) {
    int tuple1_data[] = {3, 1, 4, 1, 5};
    int tuple2_data[] = {1, 2, 3, 4, 5};
    int tuple3_data[] = {5, 4, 3, 2, 1};

    Tuple tuple1 = {tuple1_data, 5};
    Tuple tuple2 = {tuple2_data, 5};
    Tuple tuple3 = {tuple3_data, 5};

    Tuple tuples_array[] = {tuple1, tuple2, tuple3};
    TupleList list = {tuples_array, 3};

    Tuple result = tuple_intersection(&list);

    printf("Intersection size: %d\n", result.size);
    printf("Intersection elements: ");
    for (int i = 0; i < result.size; i++) {
        printf("%d ", result.elements[i]);
    }
    printf("\n");

    free(result.elements);

    int empty_tuple_data[] = {1, 2, 3};
    int empty_tuple2_data[] = {4, 5, 6};
    Tuple empty_tuple1 = {empty_tuple_data, 3};
    Tuple empty_tuple2 = {empty_tuple2_data, 3};
    Tuple empty_tuples_array[] = {empty_tuple1, empty_tuple2};
    TupleList empty_list = {empty_tuples_array, 2};

    Tuple empty_result = tuple_intersection(&empty_list);
    printf("Empty intersection size: %d\n", empty_result.size);
    free(empty_result.elements);

    Tuple single_tuple_array[] = {tuple1};
    TupleList single_list = {single_tuple_array, 1};
    Tuple single_result = tuple_intersection(&single_list);
    printf("Single tuple intersection size: %d\n", single_result.size);
    printf("Single tuple intersection elements: ");
    for (int i = 0; i < single_result.size; i++) {
        printf("%d ", single_result.elements[i]);
    }
    printf("\n");
    free(single_result.elements);

    return 0;
}