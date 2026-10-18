#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TUPLE_NAME_SIZE 50

typedef struct {
    int id;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char name[TUPLE_NAME_SIZE];
} Tuple;

static void init_tuple(Tuple *tuple, int id, const char *name) {
    if (tuple == NULL) {
        return;
    }
    tuple->id = id;
    tuple->name[0] = '\0';
    if (name == NULL) {
        return;
    }
    snprintf(tuple->name, sizeof(tuple->name), "%s", name);
    tuple->name[sizeof(tuple->name) - 1] = '\0';
}

void rearrange_tuples(Tuple *tuples, size_t tuple_count, const int *order, size_t order_count) {
    if (tuples == NULL || order == NULL || tuple_count == 0 || order_count == 0) {
        return;
    }

    if (order_count > tuple_count) {
        return;
    }

    if (tuple_count > SIZE_MAX / sizeof(Tuple)) {
        return;
    }

    Tuple *temp = malloc(tuple_count * sizeof(Tuple));
    if (temp == NULL) {
        return;
    }

    for (size_t i = 0; i < order_count; i++) {
        if (order[i] < 0 || (size_t)order[i] >= tuple_count) {
            free(temp);
            return;
        }
    }

    for (size_t i = 0; i < order_count; i++) {
        temp[i] = tuples[order[i]];
        temp[i].name[sizeof(temp[i].name) - 1] = '\0';
    }

    for (size_t i = order_count; i < tuple_count; i++) {
        temp[i] = tuples[i];
        temp[i].name[sizeof(temp[i].name) - 1] = '\0';
    }

    for (size_t i = 0; i < tuple_count; i++) {
        tuples[i] = temp[i];
    }

    free(temp);
}

int main(void) {
    Tuple tuples[4];
    init_tuple(&tuples[0], 1, "Apple");
    init_tuple(&tuples[1], 2, "Banana");
    init_tuple(&tuples[2], 3, "Cherry");
    init_tuple(&tuples[3], 4, "Date");

    int order[] = {2, 0, 3, 1};
    size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    size_t order_count = sizeof(order) / sizeof(order[0]);

    printf("Original tuples:\n");
    for (size_t i = 0; i < tuple_count; i++) {
        printf("ID: %d, Name: %.*s\n", tuples[i].id,
               (int)(sizeof(tuples[i].name) - 1), tuples[i].name);
    }

    rearrange_tuples(tuples, tuple_count, order, order_count);

    printf("\nRearranged tuples:\n");
    for (size_t i = 0; i < tuple_count; i++) {
        printf("ID: %d, Name: %.*s\n", tuples[i].id,
               (int)(sizeof(tuples[i].name) - 1), tuples[i].name);
    }

    return 0;
}