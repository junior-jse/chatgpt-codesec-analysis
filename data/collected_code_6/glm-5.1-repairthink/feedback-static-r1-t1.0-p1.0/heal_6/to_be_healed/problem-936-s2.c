#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA_LEN 4096

typedef struct {
    int id;
    char *data;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
} TupleArray;

TupleArray rearrange_tuples(const Tuple *tuples, size_t tuple_count, const int *ordered_ids, size_t id_count) {
    TupleArray result = {NULL, 0};

    if (id_count == 0 || ordered_ids == NULL) {
        return result;
    }

    result.tuples = (Tuple *)malloc(id_count * sizeof(Tuple));
    if (result.tuples == NULL) {
        return result;
    }

    result.count = id_count;

    for (size_t i = 0; i < id_count; i++) {
        int found = 0;
        if (tuples != NULL) {
            for (size_t j = 0; j < tuple_count; j++) {
                if (tuples[j].id == ordered_ids[i]) {
                    result.tuples[i].id = tuples[j].id;
                    size_t len = strnlen(tuples[j].data, MAX_DATA_LEN);
                    result.tuples[i].data = (char *)malloc(len + 1);
                    if (result.tuples[i].data != NULL) {
                        /* Possible weaknesses found:
                         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                         */
                        memcpy(result.tuples[i].data, tuples[j].data, len);
                        result.tuples[i].data[len] = '\0';
                    } else {
                        result.tuples[i].data = (char *)calloc(1, 1);
                    }
                    found = 1;
                    break;
                }
            }
        }
        if (!found) {
            result.tuples[i].id = ordered_ids[i];
            result.tuples[i].data = (char *)calloc(1, 1);
        }
    }

    return result;
}

int main(void) {
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

    for (size_t i = 0; i < rearranged.count; i++) {
        free(rearranged.tuples[i].data);
    }
    free(rearranged.tuples);

    return 0;
}