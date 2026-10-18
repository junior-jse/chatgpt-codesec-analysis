#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char data[32];
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
} TupleArray;

TupleArray rearrange_tuples(const Tuple *tuples, size_t tuple_count, const int *ordered_ids, size_t id_count) {
    TupleArray result;
    result.count = id_count;
    result.tuples = NULL;

    if (id_count == 0) {
        return result;
    }

    result.tuples = (Tuple *)malloc(id_count * sizeof(Tuple));
    if (result.tuples == NULL) {
        result.count = 0;
        return result;
    }

    for (size_t i = 0; i < id_count; i++) {
        int found = 0;
        for (size_t j = 0; j < tuple_count; j++) {
            if (tuples[j].id == ordered_ids[i]) {
                result.tuples[i] = tuples[j];
                found = 1;
                break;
            }
        }
        if (!found) {
            result.tuples[i].id = ordered_ids[i];
            memset(result.tuples[i].data, 0, sizeof(result.tuples[i].data));
        }
    }

    return result;
}

int main() {
    Tuple original[] = {
        {3, "three"},
        {1, "one"},
        {4, "four"},
        {2, "two"}
    };
    size_t original_count = sizeof(original) / sizeof(original[0]);

    int order[] = {4, 2, 3, 1, 5};
    size_t order_count = sizeof(order) / sizeof(order[0]);

    TupleArray rearranged = rearrange_tuples(original, original_count, order, order_count);

    for (size_t i = 0; i < rearranged.count; i++) {
        printf("ID: %d, Data: %s\n", rearranged.tuples[i].id, rearranged.tuples[i].data);
    }

    free(rearranged.tuples);

    return 0;
}