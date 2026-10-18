#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *elements;
    size_t size;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
} TupleList;

typedef struct {
    int *elements;
    size_t size;
    size_t capacity;
} IntArray;

static int int_array_init(IntArray *arr, size_t capacity) {
    if (arr == NULL) {
        return -1;
    }
    arr->elements = malloc(capacity * sizeof(int));
    if (arr->elements == NULL) {
        return -1;
    }
    arr->size = 0;
    arr->capacity = capacity;
    return 0;
}

static void int_array_free(IntArray *arr) {
    if (arr != NULL) {
        free(arr->elements);
        arr->elements = NULL;
        arr->size = 0;
        arr->capacity = 0;
    }
}

static int int_array_push(IntArray *arr, int value) {
    if (arr == NULL) {
        return -1;
    }
    if (arr->size >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        int *new_elements = realloc(arr->elements, new_capacity * sizeof(int));
        if (new_elements == NULL) {
            return -1;
        }
        arr->elements = new_elements;
        arr->capacity = new_capacity;
    }
    arr->elements[arr->size] = value;
    arr->size++;
    return 0;
}

static int count_occurrences(const TupleList *list, int value) {
    int count = 0;
    size_t i, j;
    
    if (list == NULL) {
        return 0;
    }
    
    for (i = 0; i < list->count; i++) {
        for (j = 0; j < list->tuples[i].size; j++) {
            if (list->tuples[i].elements[j] == value) {
                count++;
            }
        }
    }
    return count;
}

static int element_exists(const IntArray *arr, int value) {
    size_t i;
    
    if (arr == NULL) {
        return 0;
    }
    
    for (i = 0; i < arr->size; i++) {
        if (arr->elements[i] == value) {
            return 1;
        }
    }
    return 0;
}

int extract_singly_occurring(const TupleList *list, IntArray *result) {
    size_t i, j;
    int occurrences;
    
    if (list == NULL || result == NULL) {
        return -1;
    }
    
    if (int_array_init(result, 16) != 0) {
        return -1;
    }
    
    for (i = 0; i < list->count; i++) {
        for (j = 0; j < list->tuples[i].size; j++) {
            int current = list->tuples[i].elements[j];
            occurrences = count_occurrences(list, current);
            
            if (occurrences == 1 && !element_exists(result, current)) {
                if (int_array_push(result, current) != 0) {
                    int_array_free(result);
                    return -1;
                }
            }
        }
    }
    
    return 0;
}

void tuple_list_free(TupleList *list) {
    size_t i;
    
    if (list != NULL) {
        for (i = 0; i < list->count; i++) {
            free(list->tuples[i].elements);
            list->tuples[i].elements = NULL;
        }
        free(list->tuples);
        list->tuples = NULL;
        list->count = 0;
    }
}

int main(void) {
    TupleList list;
    IntArray result;
    size_t i;
    int status;
    
    list.count = 3;
    list.tuples = malloc(list.count * sizeof(Tuple));
    if (list.tuples == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    list.tuples[0].size = 3;
    list.tuples[0].elements = malloc(list.tuples[0].size * sizeof(int));
    if (list.tuples[0].elements == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(list.tuples);
        return EXIT_FAILURE;
    }
    list.tuples[0].elements[0] = 1;
    list.tuples[0].elements[1] = 2;
    list.tuples[0].elements[2] = 3;
    
    list.tuples[1].size = 3;
    list.tuples[1].elements = malloc(list.tuples[1].size * sizeof(int));
    if (list.tuples[1].elements == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(list.tuples[0].elements);
        free(list.tuples);
        return EXIT_FAILURE;
    }
    list.tuples[1].elements[0] = 2;
    list.tuples[1].elements[1] = 4;
    list.tuples[1].elements[2] = 5;
    
    list.tuples[2].size = 3;
    list.tuples[2].elements = malloc(list.tuples[2].size * sizeof(int));
    if (list.tuples[2].elements == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(list.tuples[0].elements);
        free(list.tuples[1].elements);
        free(list.tuples);
        return EXIT_FAILURE;
    }
    list.tuples[2].elements[0] = 1;
    list.tuples[2].elements[1] = 6;
    list.tuples[2].elements[2] = 7;
    
    status = extract_singly_occurring(&list, &result);
    if (status != 0) {
        fprintf(stderr, "Failed to extract singly occurring elements\n");
        tuple_list_free(&list);
        return EXIT_FAILURE;
    }
    
    printf("Elements occurring singly: ");
    for (i = 0; i < result.size; i++) {
        printf("%d ", result.elements[i]);
    }
    printf("\n");
    
    int_array_free(&result);
    tuple_list_free(&list);
    
    return EXIT_SUCCESS;
}