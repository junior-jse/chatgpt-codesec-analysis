#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int key;
    const char *value;
} Tuple;

int find_index(const int *ordered_keys, int ordered_size, int key) {
    for (int i = 0; i < ordered_size; i++) {
        if (ordered_keys[i] == key) {
            return i;
        }
    }
    return -1;
}

/* Possible weaknesses found:
 *  Parameter 'arg' can be declared as pointer to const
 *  Parameter 'arg' can be declared as pointer to const. However it seems that 'compare_tuples' is a callback function, if 'arg' is declared with const you might also need to cast function pointer(s). [constParameterCallback]
 */
int compare_tuples(const void *a, const void *b, void *arg) {
    const Tuple *ta = (const Tuple *)a;
    const Tuple *tb = (const Tuple *)b;
    const int *ordered_keys = (const int *)arg;
    int ordered_size = ordered_keys[0];
    const int *keys = ordered_keys + 1;
    
    int index_a = find_index(keys, ordered_size, ta->key);
    int index_b = find_index(keys, ordered_size, tb->key);
    
    if (index_a == -1 && index_b == -1) {
        return (ta->key > tb->key) - (ta->key < tb->key);
    }
    if (index_a == -1) {
        return 1;
    }
    if (index_b == -1) {
        return -1;
    }
    
    return (index_a > index_b) - (index_a < index_b);
}

void rearrange_tuples(Tuple *tuples, int tuple_size, const int *ordered_keys, int ordered_size) {
    if (tuples == NULL || ordered_keys == NULL || tuple_size <= 0 || ordered_size <= 0) {
        return;
    }
    
    int *arg = (int *)malloc((ordered_size + 1) * sizeof(int));
    if (arg == NULL) {
        return;
    }
    
    arg[0] = ordered_size;
    for (int i = 0; i < ordered_size; i++) {
        arg[i + 1] = ordered_keys[i];
    }
    
    /* Possible weaknesses found:
     *  passing argument 5 of 'qsort_r' from incompatible pointer type [-Wincompatible-pointer-types]
     *  You might need to cast the function pointer here
     */
    qsort_r(tuples, tuple_size, sizeof(Tuple), compare_tuples, arg);
    
    free(arg);
}

int main() {
    Tuple tuples[] = {
        {3, "three"},
        {1, "one"},
        {4, "four"},
        {2, "two"},
        {5, "five"}
    };
    int ordered_keys[] = {5, 4, 3, 2, 1};
    int tuple_size = sizeof(tuples) / sizeof(tuples[0]);
    int ordered_size = sizeof(ordered_keys) / sizeof(ordered_keys[0]);
    
    rearrange_tuples(tuples, tuple_size, ordered_keys, ordered_size);
    
    for (int i = 0; i < tuple_size; i++) {
        printf("(%d, %s)\n", tuples[i].key, tuples[i].value);
    }
    
    return 0;
}