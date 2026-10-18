#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *elements;
    int size;
} Tuple;

bool tuple_contains(Tuple *tuple, int value, bool *used) {
    for (int i = 0; i < tuple->size; i++) {
        if (!used[i] && tuple->elements[i] == value) {
            used[i] = true;
            return true;
        }
    }
    return false;
}

bool tuples_equal(Tuple *a, Tuple *b) {
    if (a->size != b->size) return false;
    
    bool *used_a = (bool *)calloc(a->size, sizeof(bool));
    bool *used_b = (bool *)calloc(b->size, sizeof(bool));
    
    if (!used_a || !used_b) {
        free(used_a);
        free(used_b);
        return false;
    }
    
    for (int i = 0; i < a->size; i++) {
        if (!tuple_contains(b, a->elements[i], used_b)) {
            free(used_a);
            free(used_b);
            return false;
        }
        if (!tuple_contains(a, b->elements[i], used_a)) {
            free(used_a);
            free(used_b);
            return false;
        }
    }
    
    free(used_a);
    free(used_b);
    return true;
}

/* Possible weaknesses found:
 *  Parameter 'elements' can be declared as pointer to const [constParameterPointer]
 */
Tuple *create_tuple(int *elements, int size) {
    Tuple *tuple = (Tuple *)malloc(sizeof(Tuple));
    if (!tuple) return NULL;
    
    tuple->elements = (int *)malloc(size * sizeof(int));
    if (!tuple->elements) {
        free(tuple);
        return NULL;
    }
    
    tuple->size = size;
    for (int i = 0; i < size; i++) {
        tuple->elements[i] = elements[i];
    }
    
    return tuple;
}

void free_tuple(Tuple *tuple) {
    if (tuple) {
        free(tuple->elements);
        free(tuple);
    }
}

/* Possible weaknesses found:
 *  expected 'Tuple *' but argument is of type 'Tuple **'
 */
Tuple *find_tuple_intersection(Tuple *list1, int size1, Tuple *list2, int size2, int *result_size) {
    int capacity = (size1 < size2) ? size1 : size2;
    Tuple *result = (Tuple *)malloc(capacity * sizeof(Tuple));
    
    if (!result) {
        *result_size = 0;
        return NULL;
    }
    
    bool *matched = (bool *)calloc(size2, sizeof(bool));
    
    if (!matched) {
        free(result);
        *result_size = 0;
        return NULL;
    }
    
    int count = 0;
    
    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            if (!matched[j] && tuples_equal(&list1[i], &list2[j])) {
                matched[j] = true;
                result[count++] = list1[i];
                break;
            }
        }
    }
    
    free(matched);
    *result_size = count;
    
    if (count == 0) {
        free(result);
        return NULL;
    }
    
    return result;
}

int main() {
    int elements1[] = {1, 2, 3};
    int elements2[] = {3, 2, 1};
    int elements3[] = {4, 5, 6};
    int elements4[] = {1, 2, 3};
    int elements5[] = {7, 8, 9};
    
    Tuple *tuple1 = create_tuple(elements1, 3);
    Tuple *tuple2 = create_tuple(elements2, 3);
    Tuple *tuple3 = create_tuple(elements3, 3);
    Tuple *tuple4 = create_tuple(elements4, 3);
    Tuple *tuple5 = create_tuple(elements5, 3);
    
    Tuple *list1[] = {tuple1, tuple3, tuple5};
    Tuple *list2[] = {tuple2, tuple4, tuple3};
    
    int result_size = 0;
    /* Possible weaknesses found:
     *  passing argument 1 of 'find_tuple_intersection' from incompatible pointer type [-Wincompatible-pointer-types]
     *  passing argument 3 of 'find_tuple_intersection' from incompatible pointer type [-Wincompatible-pointer-types]
     */
    Tuple *intersection = find_tuple_intersection(list1, 3, list2, 3, &result_size);
    
    printf("Intersection size: %d\n", result_size);
    
    for (int i = 0; i < result_size; i++) {
        printf("Tuple %d: ", i + 1);
        for (int j = 0; j < intersection[i].size; j++) {
            printf("%d ", intersection[i].elements[j]);
        }
        printf("\n");
    }
    
    free(intersection);
    free_tuple(tuple1);
    free_tuple(tuple2);
    free_tuple(tuple3);
    free_tuple(tuple4);
    free_tuple(tuple5);
    
    return 0;
}