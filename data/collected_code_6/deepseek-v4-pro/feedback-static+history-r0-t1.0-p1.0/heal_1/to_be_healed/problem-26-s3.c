#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *elements;
    int size;
} Tuple;

typedef struct {
    Tuple *tuples;
    int tuple_count;
    int k;
} TupleList;

bool tuple_list_has_all_k_elements(const TupleList *list, const int *required_elements) {
    if (list == NULL || required_elements == NULL || list->tuple_count <= 0 || list->k <= 0) {
        return false;
    }
    
    bool *found = (bool *)calloc(list->k, sizeof(bool));
    if (found == NULL) {
        return false;
    }
    
    int found_count = 0;
    int required_count = list->k;
    
    for (int i = 0; i < list->tuple_count && found_count < required_count; i++) {
        if (list->tuples[i].elements == NULL) {
            free(found);
            return false;
        }
        
        for (int j = 0; j < list->tuples[i].size && found_count < required_count; j++) {
            int element = list->tuples[i].elements[j];
            for (int req_idx = 0; req_idx < list->k; req_idx++) {
                if (!found[req_idx] && required_elements[req_idx] == element) {
                    found[req_idx] = true;
                    found_count++;
                    break;
                }
            }
        }
    }
    
    bool result = (found_count == required_count);
    free(found);
    return result;
}

int main(void) {
    int elements1[] = {1, 2, 3};
    int elements2[] = {4, 5};
    int elements3[] = {6};
    
    Tuple tuples[] = {
        {elements1, 3},
        {elements2, 2},
        {elements3, 1}
    };
    
    TupleList list = {
        tuples,
        3,
        6
    };
    
    /* Possible weaknesses found:
     *  Variable 'required_elements' can be declared as const array [constVariable]
     */
    int required_elements[] = {1, 2, 3, 4, 5, 6};
    
    if (tuple_list_has_all_k_elements(&list, required_elements)) {
        printf("All elements found\n");
    } else {
        printf("Not all elements found\n");
    }
    
    return 0;
}