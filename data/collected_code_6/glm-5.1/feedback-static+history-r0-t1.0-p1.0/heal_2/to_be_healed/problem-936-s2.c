#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char data[32];
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
} TupleArray;

TupleArray rearrange_tuples(const Tuple *tuples, size_t tuple_count, const int *ordered_ids, size_t id_count) {
    TupleArray result;
    result.count = 0;
    result.tuples = NULL;

    if (id_count == 0 || ordered_ids == NULL) {
        return result;
    }

    if (tuples == NULL && tuple_count > 0) {
        return result;
    }

    result.tuples = (Tuple *)calloc(id_count, sizeof(Tuple));
    if (result.tuples == NULL) {
        return result;
    }
    result.count = id_count;

    for (size_t i = 0; i < id_count; i++) {
        result.tuples[i].id = ordered_ids[i];
        int found = 0;
        for (size_t j = 0; j < tuple_count; j++) {
            if (tuples[j].id == ordered_ids[i]) {
                result.tuples[i].id = tuples[j].id;
                /* Possible weaknesses found:
                 * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
                 */
                strncpy(result.tuples[i].data, tuples[j].data, sizeof(result.tuples[i].data) - 1);
                result.tuples[i].data[sizeof(result.tuples[i].data) - 1] = '\0';
                found = 1;
                break;
            }
        }
        if (!found) {
            result.tuples[i].data[0] = '\0';
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