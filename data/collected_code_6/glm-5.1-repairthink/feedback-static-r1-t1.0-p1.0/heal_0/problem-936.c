#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[32];
} Tuple;

Tuple* rearrange_tuples(Tuple* tuples, size_t tuple_count, const int* order, size_t order_count) {
    if (tuples == NULL || order == NULL) {
        return NULL;
    }

    Tuple* result = (Tuple*)malloc(tuple_count * sizeof(Tuple));
    if (result == NULL) {
        return NULL;
    }

    size_t result_idx = 0;

    for (size_t i = 0; i < order_count; ++i) {
        for (size_t j = 0; j < tuple_count; ++j) {
            if (tuples[j].id == order[i]) {
                result[result_idx++] = tuples[j];
                break;
            }
        }
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        int found = 0;
        for (size_t j = 0; j < order_count; ++j) {
            if (tuples[i].id == order[j]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            result[result_idx++] = tuples[i];
        }
    }

    return result;
}

int main() {
    Tuple tuples[] = {
        {2, "Bob"},
        {1, "Alice"},
        {4, "David"},
        {3, "Charlie"}
    };

    int order[] = {3, 1, 4};
    
    size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    size_t order_count = sizeof(order) / sizeof(order[0]);

    Tuple* rearranged = rearrange_tuples(tuples, tuple_count, order, order_count);
    if (rearranged == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        printf("%d: %s\n", rearranged[i].id, rearranged[i].name);
    }

    free(rearranged);
    return EXIT_SUCCESS;
}