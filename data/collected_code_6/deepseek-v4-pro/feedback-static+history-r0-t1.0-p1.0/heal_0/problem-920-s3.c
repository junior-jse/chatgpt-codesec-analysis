#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Tuple {
    void **values;
    size_t size;
} Tuple;

typedef struct TupleList {
    Tuple *tuples;
    size_t count;
} TupleList;

bool is_none(void *value) {
    return value == NULL;
}

TupleList *remove_all_none_tuples(TupleList *list) {
    if (list == NULL || list->tuples == NULL) {
        return list;
    }

    size_t write_index = 0;
    
    for (size_t i = 0; i < list->count; i++) {
        bool all_none = true;
        
        for (size_t j = 0; j < list->tuples[i].size; j++) {
            if (!is_none(list->tuples[i].values[j])) {
                all_none = false;
                break;
            }
        }
        
        if (!all_none) {
            if (write_index != i) {
                list->tuples[write_index] = list->tuples[i];
            }
            write_index++;
        } else {
            free(list->tuples[i].values);
        }
    }
    
    list->count = write_index;
    
    if (write_index == 0) {
        free(list->tuples);
        list->tuples = NULL;
    } else {
        Tuple *new_tuples = (Tuple *)realloc(list->tuples, write_index * sizeof(Tuple));
        if (new_tuples != NULL) {
            list->tuples = new_tuples;
        }
    }
    
    return list;
}

Tuple *create_tuple(void **values, size_t size) {
    Tuple *tuple = (Tuple *)malloc(sizeof(Tuple));
    if (tuple == NULL) {
        return NULL;
    }
    
    tuple->values = (void **)malloc(size * sizeof(void *));
    if (tuple->values == NULL) {
        free(tuple);
        return NULL;
    }
    
    memcpy(tuple->values, values, size * sizeof(void *));
    tuple->size = size;
    
    return tuple;
}

TupleList *create_tuple_list(Tuple *tuples, size_t count) {
    TupleList *list = (TupleList *)malloc(sizeof(TupleList));
    if (list == NULL) {
        return NULL;
    }
    
    list->tuples = tuples;
    list->count = count;
    
    return list;
}

void free_tuple_list(TupleList *list) {
    if (list == NULL) {
        return;
    }
    
    if (list->tuples != NULL) {
        for (size_t i = 0; i < list->count; i++) {
            free(list->tuples[i].values);
        }
        free(list->tuples);
    }
    
    free(list);
}

int main(void) {
    void *v1[] = {NULL, NULL, NULL};
    void *v2[] = {(void *)"hello", NULL, (void *)"world"};
    void *v3[] = {NULL, (void *)"value", NULL};
    void *v4[] = {NULL, NULL, NULL};
    void *v5[] = {(void *)"a", (void *)"b", (void *)"c"};
    
    Tuple *tuples = (Tuple *)malloc(5 * sizeof(Tuple));
    if (tuples == NULL) {
        return 1;
    }
    
    Tuple *t1 = create_tuple(v1, 3);
    Tuple *t2 = create_tuple(v2, 3);
    Tuple *t3 = create_tuple(v3, 3);
    Tuple *t4 = create_tuple(v4, 3);
    Tuple *t5 = create_tuple(v5, 3);
    
    if (t1 == NULL || t2 == NULL || t3 == NULL || t4 == NULL || t5 == NULL) {
        free(t1);
        free(t2);
        free(t3);
        free(t4);
        free(t5);
        free(tuples);
        return 1;
    }
    
    tuples[0] = *t1;
    tuples[1] = *t2;
    tuples[2] = *t3;
    tuples[3] = *t4;
    tuples[4] = *t5;
    
    free(t1);
    free(t2);
    free(t3);
    free(t4);
    free(t5);
    
    TupleList *list = create_tuple_list(tuples, 5);
    if (list == NULL) {
        free_tuple_list(list);
        return 1;
    }
    
    printf("Original count: %zu\n", list->count);
    
    remove_all_none_tuples(list);
    
    printf("After removal count: %zu\n", list->count);
    
    for (size_t i = 0; i < list->count; i++) {
        printf("Tuple %zu: ", i);
        for (size_t j = 0; j < list->tuples[i].size; j++) {
            if (list->tuples[i].values[j] == NULL) {
                printf("NULL ");
            } else {
                printf("%s ", (char *)list->tuples[i].values[j]);
            }
        }
        printf("\n");
    }
    
    free_tuple_list(list);
    
    return 0;
}