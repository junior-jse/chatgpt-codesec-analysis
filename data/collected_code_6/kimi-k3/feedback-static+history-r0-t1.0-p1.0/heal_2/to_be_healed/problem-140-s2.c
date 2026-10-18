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

typedef struct {
    Tuple *elements;
    size_t count;
    size_t capacity;
} SingleElements;

static int compare_tuples(const void *a, const void *b) {
    const Tuple *ta = (const Tuple *)a;
    const Tuple *tb = (const Tuple *)b;
    
    if (ta->size != tb->size) {
        return (ta->size < tb->size) ? -1 : 1;
    }
    
    for (size_t i = 0; i < ta->size; i++) {
        if (ta->data[i] != tb->data[i]) {
            return (ta->data[i] < tb->data[i]) ? -1 : 1;
        }
    }
    
    return 0;
}

static int tuple_equals(const Tuple *a, const Tuple *b) {
    if (a->size != b->size) {
        return 0;
    }
    
    for (size_t i = 0; i < a->size; i++) {
        if (a->data[i] != b->data[i]) {
            return 0;
        }
    }
    
    return 1;
}

static int add_to_singles(SingleElements *singles, const Tuple *tuple) {
    if (singles->count >= singles->capacity) {
        size_t new_capacity = singles->capacity == 0 ? 8 : singles->capacity * 2;
        Tuple *new_elements = realloc(singles->elements, new_capacity * sizeof(Tuple));
        if (new_elements == NULL) {
            return -1;
        }
        singles->elements = new_elements;
        singles->capacity = new_capacity;
    }
    
    singles->elements[singles->count].data = malloc(tuple->size * sizeof(int));
    if (singles->elements[singles->count].data == NULL) {
        return -1;
    }
    
    if (tuple->size > 0 && tuple->data != NULL) {
        /* Possible weaknesses found:
         *  'copy_size' is assigned value 'tuple->size*sizeof(int)' here.
         */
        size_t copy_size = tuple->size * sizeof(int);
        /* Possible weaknesses found:
         *  The comparison 'copy_size <= tuple->size*sizeof(int)' is always true because 'copy_size' and 'tuple->size*sizeof(int)' represent the same value. [knownConditionTrueFalse]
         *  The comparison 'copy_size <= tuple->size*sizeof(int)' is always true because 'copy_size' and 'tuple->size*sizeof(int)' represent the same value.
         */
        if (copy_size <= tuple->size * sizeof(int)) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(singles->elements[singles->count].data, tuple->data, copy_size);
        }
    }
    singles->elements[singles->count].size = tuple->size;
    singles->count++;
    
    return 0;
}

SingleElements extract_single_occurrences(const TupleList *list) {
    SingleElements result = {NULL, 0, 0};
    
    /* Possible weaknesses found:
     *  Assuming that condition 'list->count==0' is not redundant
     *  Assuming condition 'list->tuples==NULL' is false
     */
    if (list == NULL || list->tuples == NULL || list->count == 0) {
        return result;
    }
    
    Tuple *sorted = malloc(list->count * sizeof(Tuple));
    /* Possible weaknesses found:
     *  Assuming condition is false
     */
    if (sorted == NULL) {
        return result;
    }
    
    /* Possible weaknesses found:
     *  Condition 'list->tuples!=NULL' is always true
     *  Condition 'list->count>0' is always true
     *  Condition 'list->count>0' is always true [knownConditionTrueFalse]
     *  Condition 'list->tuples!=NULL' is always true [knownConditionTrueFalse]
     */
    if (list->count > 0 && list->tuples != NULL) {
        /* Possible weaknesses found:
         *  'copy_size' is assigned value 'list->count*sizeof(struct Tuple)' here.
         */
        size_t copy_size = list->count * sizeof(Tuple);
        /* Possible weaknesses found:
         *  The comparison 'copy_size <= list->count*sizeof(struct Tuple)' is always true because 'copy_size' and 'list->count*sizeof(struct Tuple)' represent the same value.
         *  The comparison 'copy_size <= list->count*sizeof(struct Tuple)' is always true because 'copy_size' and 'list->count*sizeof(struct Tuple)' represent the same value. [knownConditionTrueFalse]
         */
        if (copy_size <= list->count * sizeof(Tuple)) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(sorted, list->tuples, copy_size);
        }
    }
    qsort(sorted, list->count, sizeof(Tuple), compare_tuples);
    
    size_t i = 0;
    while (i < list->count) {
        size_t j = i;
        while (j < list->count && tuple_equals(&sorted[i], &sorted[j])) {
            j++;
        }
        
        if (j - i == 1) {
            if (add_to_singles(&result, &sorted[i]) != 0) {
                for (size_t k = 0; k < result.count; k++) {
                    free(result.elements[k].data);
                }
                free(result.elements);
                free(sorted);
                result.elements = NULL;
                result.count = 0;
                result.capacity = 0;
                return result;
            }
        }
        
        i = j;
    }
    
    free(sorted);
    return result;
}

void free_single_elements(SingleElements *singles) {
    if (singles == NULL) {
        return;
    }
    
    for (size_t i = 0; i < singles->count; i++) {
        free(singles->elements[i].data);
    }
    free(singles->elements);
    singles->elements = NULL;
    singles->count = 0;
    singles->capacity = 0;
}

void free_tuple_list(TupleList *list) {
    if (list == NULL) {
        return;
    }
    
    for (size_t i = 0; i < list->count; i++) {
        free(list->tuples[i].data);
    }
    free(list->tuples);
    list->tuples = NULL;
    list->count = 0;
}

int create_tuple(Tuple *tuple, const int *data, size_t size) {
    if (tuple == NULL || (data == NULL && size > 0)) {
        return -1;
    }
    
    tuple->data = malloc(size * sizeof(int));
    if (tuple->data == NULL && size > 0) {
        return -1;
    }
    
    if (size > 0 && data != NULL) {
        /* Possible weaknesses found:
         *  'copy_size' is assigned value 'size*sizeof(int)' here.
         */
        size_t copy_size = size * sizeof(int);
        /* Possible weaknesses found:
         *  The comparison 'copy_size <= size*sizeof(int)' is always true because 'copy_size' and 'size*sizeof(int)' represent the same value.
         *  The comparison 'copy_size <= size*sizeof(int)' is always true because 'copy_size' and 'size*sizeof(int)' represent the same value. [knownConditionTrueFalse]
         */
        if (copy_size <= size * sizeof(int)) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(tuple->data, data, copy_size);
        }
    }
    tuple->size = size;
    
    return 0;
}

int main(void) {
    TupleList list = {NULL, 0};
    list.count = 6;
    list.tuples = calloc(list.count, sizeof(Tuple));
    
    if (list.tuples == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    const int data0[] = {1, 2, 3};
    const int data1[] = {4, 5};
    const int data2[] = {1, 2, 3};
    const int data3[] = {6};
    const int data4[] = {4, 5};
    const int data5[] = {7, 8, 9};
    
    if (create_tuple(&list.tuples[0], data0, 3) != 0 ||
        create_tuple(&list.tuples[1], data1, 2) != 0 ||
        create_tuple(&list.tuples[2], data2, 3) != 0 ||
        create_tuple(&list.tuples[3], data3, 1) != 0 ||
        create_tuple(&list.tuples[4], data4, 2) != 0 ||
        create_tuple(&list.tuples[5], data5, 3) != 0) {
        fprintf(stderr, "Failed to create tuples\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }
    
    SingleElements singles = extract_single_occurrences(&list);
    
    printf("Elements occurring singly:\n");
    for (size_t i = 0; i < singles.count; i++) {
        printf("(");
        for (size_t j = 0; j < singles.elements[i].size; j++) {
            printf("%d", singles.elements[i].data[j]);
            if (j < singles.elements[i].size - 1) {
                printf(", ");
            }
        }
        printf(")\n");
    }
    
    free_single_elements(&singles);
    free_tuple_list(&list);
    
    return EXIT_SUCCESS;
}