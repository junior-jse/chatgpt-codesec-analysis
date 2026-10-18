#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum data_type { INT_TYPE, FLOAT_TYPE, CHAR_TYPE };

typedef struct {
    enum data_type type;
    void *data;
} TupleElement;

typedef struct {
    TupleElement *elements;
    size_t size;
} Tuple;

Tuple remove_by_type(Tuple tuple, enum data_type target_type) {
    Tuple result;
    result.size = 0;
    result.elements = (TupleElement *)malloc(tuple.size * sizeof(TupleElement));
    if (tuple.size > 0 && result.elements == NULL) {
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < tuple.size; i++) {
        if (tuple.elements[i].type != target_type) {
            result.elements[result.size].type = tuple.elements[i].type;
            switch (tuple.elements[i].type) {
                case INT_TYPE: {
                    int *val = (int *)malloc(sizeof(int));
                    if (val == NULL) exit(EXIT_FAILURE);
                    *val = *(int *)tuple.elements[i].data;
                    result.elements[result.size].data = val;
                    break;
                }
                case FLOAT_TYPE: {
                    float *val = (float *)malloc(sizeof(float));
                    if (val == NULL) exit(EXIT_FAILURE);
                    *val = *(float *)tuple.elements[i].data;
                    result.elements[result.size].data = val;
                    break;
                }
                case CHAR_TYPE: {
                    char *val = (char *)malloc(sizeof(char));
                    if (val == NULL) exit(EXIT_FAILURE);
                    *val = *(char *)tuple.elements[i].data;
                    result.elements[result.size].data = val;
                    break;
                }
            }
            result.size++;
        }
    }

    if (result.size == 0) {
        free(result.elements);
        result.elements = NULL;
    } else {
        TupleElement *temp = (TupleElement *)realloc(result.elements, result.size * sizeof(TupleElement));
        if (temp == NULL) {
            free(result.elements);
            exit(EXIT_FAILURE);
        }
        result.elements = temp;
    }

    return result;
}

void free_tuple(Tuple *tuple) {
    if (tuple != NULL) {
        for (size_t i = 0; i < tuple->size; i++) {
            free(tuple->elements[i].data);
        }
        free(tuple->elements);
        tuple->elements = NULL;
        tuple->size = 0;
    }
}

int main(void) {
    Tuple tuple;
    tuple.size = 5;
    tuple.elements = (TupleElement *)malloc(tuple.size * sizeof(TupleElement));
    if (tuple.elements == NULL) {
        return EXIT_FAILURE;
    }

    tuple.elements[0].type = INT_TYPE;
    tuple.elements[0].data = malloc(sizeof(int));
    if (tuple.elements[0].data == NULL) return EXIT_FAILURE;
    *(int *)tuple.elements[0].data = 10;

    tuple.elements[1].type = FLOAT_TYPE;
    tuple.elements[1].data = malloc(sizeof(float));
    if (tuple.elements[1].data == NULL) return EXIT_FAILURE;
    *(float *)tuple.elements[1].data = 3.14f;

    tuple.elements[2].type = CHAR_TYPE;
    tuple.elements[2].data = malloc(sizeof(char));
    if (tuple.elements[2].data == NULL) return EXIT_FAILURE;
    *(char *)tuple.elements[2].data = 'A';

    tuple.elements[3].type = INT_TYPE;
    tuple.elements[3].data = malloc(sizeof(int));
    if (tuple.elements[3].data == NULL) return EXIT_FAILURE;
    *(int *)tuple.elements[3].data = 20;

    tuple.elements[4].type = CHAR_TYPE;
    tuple.elements[4].data = malloc(sizeof(char));
    if (tuple.elements[4].data == NULL) return EXIT_FAILURE;
    *(char *)tuple.elements[4].data = 'B';

    Tuple filtered = remove_by_type(tuple, INT_TYPE);

    for (size_t i = 0; i < filtered.size; i++) {
        switch (filtered.elements[i].type) {
            case INT_TYPE:
                printf("%d ", *(int *)filtered.elements[i].data);
                break;
            case FLOAT_TYPE:
                printf("%f ", *(float *)filtered.elements[i].data);
                break;
            case CHAR_TYPE:
                printf("%c ", *(char *)filtered.elements[i].data);
                break;
        }
    }
    printf("\n");

    free_tuple(&tuple);
    free_tuple(&filtered);

    return EXIT_SUCCESS;
}