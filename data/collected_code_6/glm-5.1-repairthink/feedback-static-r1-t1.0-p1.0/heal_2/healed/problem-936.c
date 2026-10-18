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

    int* added = (int*)calloc(tuple_count, sizeof(int));
    if (added == NULL) {
        free(result);
        return NULL;
    }

    size_t result_idx = 0;

    for (size_t i = 0; i < order_count; ++i) {
        for (size_t j = 0; j < tuple_count; ++j) {
            if (!added[j] && tuples[j].id == order[i]) {
                result[result_idx].id = tuples[j].id;
                snprintf(result[result_idx].name, sizeof(result[result_idx].name), "%s", tuples[j].name);
                result_idx++;
                added[j] = 1;
                break;
            }
        }
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        if (!added[i]) {
            result[result_idx].id = tuples[i].id;
            snprintf(result[result_idx].name, sizeof(result[result_idx].name), "%s", tuples[i].name);
            result_idx++;
        }
    }

    free(added);
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