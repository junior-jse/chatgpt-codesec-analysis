#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

typedef struct {
    int *elements;
    size_t size;
} Tuple;

typedef struct {
    int *elements;
    size_t size;
} Set;

int compare_ints(const void *a, const void *b) {
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

Set tuple_to_set(const Tuple *tuple) {
    Set result = {NULL, 0};
    
    if (tuple == NULL || tuple->elements == NULL || tuple->size == 0) {
        return result;
    }

    if (tuple->size > SIZE_MAX / sizeof(int)) {
        return result;
    }

    size_t copy_size = tuple->size * sizeof(int);
    int *temp = malloc(copy_size);
    if (temp == NULL) {
        return result;
    }

    if (tuple->size > 0 && copy_size / sizeof(int) != tuple->size) {
        free(temp);
        return result;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(temp, tuple->elements, copy_size);
    
    qsort(temp, tuple->size, sizeof(int), compare_ints);

    size_t unique_count = 0;
    for (size_t i = 0; i < tuple->size; i++) {
        if (i == 0 || temp[i] != temp[i - 1]) {
            temp[unique_count++] = temp[i];
        }
    }

    if (unique_count == 0) {
        free(temp);
        return result;
    }

    if (unique_count > SIZE_MAX / sizeof(int)) {
        free(temp);
        return result;
    }

    size_t final_size = unique_count * sizeof(int);
    if (final_size / sizeof(int) != unique_count) {
        free(temp);
        return result;
    }

    int *final_elements = realloc(temp, final_size);
    if (final_elements == NULL) {
        result.elements = temp;
        result.size = unique_count;
    } else {
        result.elements = final_elements;
        result.size = unique_count;
    }

    return result;
}

void free_set(Set *set) {
    if (set != NULL) {
        free(set->elements);
        set->elements = NULL;
        set->size = 0;
    }
}

void free_tuple(Tuple *tuple) {
    if (tuple != NULL) {
        free(tuple->elements);
        tuple->elements = NULL;
        tuple->size = 0;
    }
}

int main(void) {
    int data[] = {5, 2, 8, 2, 9, 1, 5, 5, 8};
    /* Possible weaknesses found:
     *  Assignment 'data_size=sizeof(data)/sizeof(data[0])', assigned value is 9
     */
    size_t data_size = sizeof(data) / sizeof(data[0]);
    
    /* Possible weaknesses found:
     *  Assuming condition is false
     */
    if (data_size > SIZE_MAX / sizeof(int)) {
        return EXIT_FAILURE;
    }
    
    Tuple input = {NULL, 0};
    /* Possible weaknesses found:
     *  Assignment 'alloc_size=data_size*sizeof(int)', assigned value is 36
     */
    size_t alloc_size = data_size * sizeof(int);
    
    /* Possible weaknesses found:
     *  Assuming that condition 'alloc_size/sizeof(int)!=data_size' is not redundant
     */
    if (alloc_size / sizeof(int) != data_size) {
        return EXIT_FAILURE;
    }
    
    input.elements = malloc(alloc_size);
    /* Possible weaknesses found:
     *  Assuming condition is false
     */
    if (input.elements == NULL) {
        return EXIT_FAILURE;
    }
    
    /* Possible weaknesses found:
     *  Condition 'data_size>0' is always true [knownConditionTrueFalse]
     *  Condition 'data_size>0' is always true
     */
    if (data_size > 0 && alloc_size / sizeof(int) != data_size) {
        free(input.elements);
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(input.elements, data, alloc_size);
    input.size = data_size;

    Set output = tuple_to_set(&input);

    if (output.elements != NULL) {
        printf("{ ");
        for (size_t i = 0; i < output.size; i++) {
            printf("%d ", output.elements[i]);
        }
        printf("}\n");
    }

    free_set(&output);
    free_tuple(&input);

    return EXIT_SUCCESS;
}