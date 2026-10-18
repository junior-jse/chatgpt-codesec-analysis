#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *elements;
    size_t size;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t size;
    size_t capacity;
} TupleList;

void init_tuple_list(TupleList *list) {
    list->tuples = NULL;
    list->size = 0;
    list->capacity = 0;
}

int add_tuple(TupleList *list, const int *elements, size_t size) {
    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 4 : list->capacity * 2;
        Tuple *new_tuples = realloc(list->tuples, new_capacity * sizeof(Tuple));
        if (new_tuples == NULL) {
            return -1;
        }
        list->tuples = new_tuples;
        list->capacity = new_capacity;
    }
    
    list->tuples[list->size].elements = malloc(size * sizeof(int));
    if (list->tuples[list->size].elements == NULL) {
        return -1;
    }
    
    memcpy(list->tuples[list->size].elements, elements, size * sizeof(int));
    list->tuples[list->size].size = size;
    list->size++;
    return 0;
}

void free_tuple_list(TupleList *list) {
    for (size_t i = 0; i < list->size; i++) {
        free(list->tuples[i].elements);
    }
    free(list->tuples);
    list->tuples = NULL;
    list->size = 0;
    list->capacity = 0;
}

int join_tuples_by_initial(const TupleList *input, TupleList *output) {
    if (input == NULL || output == NULL || input->size == 0) {
        return -1;
    }
    
    init_tuple_list(output);
    
    int *processed = calloc(input->size, sizeof(int));
    if (processed == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < input->size; i++) {
        if (processed[i] || input->tuples[i].size == 0) {
            continue;
        }
        
        int initial = input->tuples[i].elements[0];
        size_t total_size = 0;
        
        for (size_t j = i; j < input->size; j++) {
            if (!processed[j] && input->tuples[j].size > 0 && 
                input->tuples[j].elements[0] == initial) {
                total_size += input->tuples[j].size;
            }
        }
        
        int *joined = malloc(total_size * sizeof(int));
        if (joined == NULL) {
            free(processed);
            free_tuple_list(output);
            return -1;
        }
        
        size_t offset = 0;
        for (size_t j = i; j < input->size; j++) {
            if (!processed[j] && input->tuples[j].size > 0 && 
                input->tuples[j].elements[0] == initial) {
                memcpy(joined + offset, input->tuples[j].elements, 
                       input->tuples[j].size * sizeof(int));
                offset += input->tuples[j].size;
                processed[j] = 1;
            }
        }
        
        if (add_tuple(output, joined, total_size) != 0) {
            free(joined);
            free(processed);
            free_tuple_list(output);
            return -1;
        }
        
        free(joined);
    }
    
    free(processed);
    return 0;
}

void print_tuple_list(const TupleList *list) {
    for (size_t i = 0; i < list->size; i++) {
        printf("(");
        for (size_t j = 0; j < list->tuples[i].size; j++) {
            printf("%d", list->tuples[i].elements[j]);
            if (j < list->tuples[i].size - 1) {
                printf(", ");
            }
        }
        printf(")\n");
    }
}

int main(void) {
    TupleList input, output;
    init_tuple_list(&input);
    
    int t1[] = {1, 2, 3};
    int t2[] = {4, 5};
    int t3[] = {1, 6, 7};
    int t4[] = {4, 8, 9};
    int t5[] = {2, 10};
    
    if (add_tuple(&input, t1, 3) != 0 ||
        add_tuple(&input, t2, 2) != 0 ||
        add_tuple(&input, t3, 3) != 0 ||
        add_tuple(&input, t4, 3) != 0 ||
        add_tuple(&input, t5, 2) != 0) {
        free_tuple_list(&input);
        return EXIT_FAILURE;
    }
    
    printf("Input tuples:\n");
    print_tuple_list(&input);
    
    if (join_tuples_by_initial(&input, &output) != 0) {
        free_tuple_list(&input);
        return EXIT_FAILURE;
    }
    
    printf("\nJoined tuples:\n");
    print_tuple_list(&output);
    
    free_tuple_list(&input);
    free_tuple_list(&output);
    
    return EXIT_SUCCESS;
}