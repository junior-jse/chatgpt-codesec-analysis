#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *elements;
    int size;
} Tuple;

bool has_all_elements(Tuple *tuples, int tuple_count, int k) {
    if (tuples == NULL || k <= 0 || tuple_count <= 0) {
        return false;
    }
    
    bool *seen = (bool *)calloc(k + 1, sizeof(bool));
    if (seen == NULL) {
        return false;
    }
    
    for (int i = 0; i < tuple_count; i++) {
        if (tuples[i].elements == NULL || tuples[i].size <= 0) {
            free(seen);
            return false;
        }
        for (int j = 0; j < tuples[i].size; j++) {
            if (tuples[i].elements[j] >= 1 && tuples[i].elements[j] <= k) {
                seen[tuples[i].elements[j]] = true;
            }
        }
    }
    
    for (int i = 1; i <= k; i++) {
        if (!seen[i]) {
            free(seen);
            return false;
        }
    }
    
    free(seen);
    return true;
}

int main(void) {
    int elems1[] = {1, 2, 3};
    int elems2[] = {4, 5};
    int elems3[] = {6, 1};
    
    Tuple tuples[3];
    tuples[0].elements = elems1;
    tuples[0].size = 3;
    tuples[1].elements = elems2;
    tuples[1].size = 2;
    tuples[2].elements = elems3;
    tuples[2].size = 2;
    
    if (has_all_elements(tuples, 3, 6)) {
        printf("All elements present\n");
    } else {
        printf("Not all elements present\n");
    }
    
    int elems4[] = {1, 2};
    int elems5[] = {3, 4};
    
    Tuple tuples2[2];
    tuples2[0].elements = elems4;
    tuples2[0].size = 2;
    tuples2[1].elements = elems5;
    tuples2[1].size = 2;
    
    if (has_all_elements(tuples2, 2, 5)) {
        printf("All elements present\n");
    } else {
        printf("Not all elements present\n");
    }
    
    return 0;
}