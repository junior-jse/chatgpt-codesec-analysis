#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int count;
} ElementCount;

typedef struct {
    int *elements;
    int size;
} Tuple;

int *extract_single_occurrences(Tuple *tuples, int tuple_count, int *result_size) {
    if (tuples == NULL || tuple_count <= 0 || result_size == NULL) {
        *result_size = 0;
        return NULL;
    }

    int total_elements = 0;
    for (int i = 0; i < tuple_count; i++) {
        if (tuples[i].elements == NULL || tuples[i].size < 0) {
            *result_size = 0;
            return NULL;
        }
        total_elements += tuples[i].size;
    }

    ElementCount *counts = (ElementCount *)malloc(total_elements * sizeof(ElementCount));
    if (counts == NULL) {
        *result_size = 0;
        return NULL;
    }

    int unique_count = 0;
    for (int i = 0; i < tuple_count; i++) {
        for (int j = 0; j < tuples[i].size; j++) {
            int value = tuples[i].elements[j];
            int found = 0;
            for (int k = 0; k < unique_count; k++) {
                if (counts[k].value == value) {
                    counts[k].count++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                counts[unique_count].value = value;
                counts[unique_count].count = 1;
                unique_count++;
            }
        }
    }

    int *singles = (int *)malloc(unique_count * sizeof(int));
    if (singles == NULL) {
        free(counts);
        *result_size = 0;
        return NULL;
    }

    int single_count = 0;
    for (int i = 0; i < unique_count; i++) {
        if (counts[i].count == 1) {
            singles[single_count++] = counts[i].value;
        }
    }

    free(counts);

    if (single_count == 0) {
        free(singles);
        *result_size = 0;
        return NULL;
    }

    int *result = (int *)malloc(single_count * sizeof(int));
    if (result == NULL) {
        free(singles);
        *result_size = 0;
        return NULL;
    }

    for (int i = 0; i < single_count; i++) {
        result[i] = singles[i];
    }

    free(singles);
    *result_size = single_count;
    return result;
}

int main(void) {
    int elements1[] = {1, 2, 3, 2};
    int elements2[] = {4, 5, 1, 6};
    int elements3[] = {7, 8, 9, 7};

    Tuple tuples[3];
    tuples[0].elements = elements1;
    tuples[0].size = 4;
    tuples[1].elements = elements2;
    tuples[1].size = 4;
    tuples[2].elements = elements3;
    tuples[2].size = 4;

    int result_size = 0;
    int *result = extract_single_occurrences(tuples, 3, &result_size);

    if (result != NULL) {
        printf("Single occurrences: ");
        for (int i = 0; i < result_size; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    } else {
        printf("No single occurrences found.\n");
    }

    int empty_elements[] = {};
    Tuple empty_tuple;
    empty_tuple.elements = empty_elements;
    empty_tuple.size = 0;

    int empty_result_size = 0;
    int *empty_result = extract_single_occurrences(&empty_tuple, 1, &empty_result_size);
    if (empty_result == NULL) {
        printf("Empty test passed.\n");
    } else {
        free(empty_result);
    }

    return 0;
}