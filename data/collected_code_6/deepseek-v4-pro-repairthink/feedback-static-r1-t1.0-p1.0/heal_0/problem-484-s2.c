#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
} Tuple;

int contains(const Tuple *tuple, int value) {
    for (size_t i = 0; i < tuple->size; i++) {
        if (tuple->data[i] == value) {
            return 1;
        }
    }
    return 0;
}

Tuple remove_matching_tuples(const Tuple *tuple1, const Tuple *tuple2) {
    Tuple result;
    result.data = NULL;
    result.size = 0;

    if (tuple1 == NULL || tuple2 == NULL || tuple1->data == NULL || tuple2->data == NULL) {
        return result;
    }

    for (size_t i = 0; i < tuple1->size; i++) {
        if (!contains(tuple2, tuple1->data[i])) {
            int *new_data = realloc(result.data, (result.size + 1) * sizeof(int));
            if (new_data == NULL) {
                free(result.data);
                result.data = NULL;
                result.size = 0;
                return result;
            }
            result.data = new_data;
            result.data[result.size] = tuple1->data[i];
            result.size++;
        }
    }

    for (size_t i = 0; i < tuple2->size; i++) {
        if (!contains(tuple1, tuple2->data[i])) {
            int *new_data = realloc(result.data, (result.size + 1) * sizeof(int));
            if (new_data == NULL) {
                free(result.data);
                result.data = NULL;
                result.size = 0;
                return result;
            }
            result.data = new_data;
            result.data[result.size] = tuple2->data[i];
            result.size++;
        }
    }

    return result;
}

void print_tuple(const Tuple *tuple) {
    if (tuple == NULL || tuple->data == NULL) {
        printf("()\n");
        return;
    }
    printf("(");
    for (size_t i = 0; i < tuple->size; i++) {
        printf("%d", tuple->data[i]);
        if (i < tuple->size - 1) {
            printf(", ");
        }
    }
    printf(")\n");
}

int main(void) {
    int data1[] = {1, 2, 3, 4, 5};
    int data2[] = {3, 4, 5, 6, 7};
    
    Tuple tuple1 = {data1, 5};
    Tuple tuple2 = {data2, 5};
    
    printf("Tuple 1: ");
    print_tuple(&tuple1);
    printf("Tuple 2: ");
    print_tuple(&tuple2);
    
    Tuple result = remove_matching_tuples(&tuple1, &tuple2);
    
    if (result.data == NULL && result.size == 0) {
        printf("Result: ()\n");
    } else {
        printf("Result: ");
        print_tuple(&result);
        free(result.data);
    }
    
    return 0;
}