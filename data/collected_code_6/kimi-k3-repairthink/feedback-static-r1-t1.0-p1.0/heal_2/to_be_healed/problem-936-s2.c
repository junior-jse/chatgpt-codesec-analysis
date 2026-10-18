#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define NAME_SIZE 50

typedef struct {
    int id;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char name[NAME_SIZE];
    float value;
} Tuple;

static int tuple_init(Tuple *tuple, int id, const char *name, float value)
{
    int written;

    if (tuple == NULL || name == NULL) {
        return -1;
    }

    tuple->id = id;
    tuple->value = value;
    tuple->name[0] = '\0';

    written = snprintf(tuple->name, NAME_SIZE, "%s", name);
    if (written < 0 || (size_t)written >= NAME_SIZE) {
        tuple->name[0] = '\0';
        return -1;
    }

    return 0;
}

int rearrange_tuples(Tuple *tuples, size_t tuple_count, const size_t *order, size_t order_count)
{
    Tuple *temp;
    size_t i;

    if (tuples == NULL || order == NULL) {
        return -1;
    }

    if (tuple_count == 0 || order_count == 0) {
        return -1;
    }

    if (order_count > tuple_count) {
        return -1;
    }

    if (tuple_count > SIZE_MAX / sizeof(Tuple)) {
        return -1;
    }

    temp = (Tuple *)malloc(tuple_count * sizeof(Tuple));
    if (temp == NULL) {
        return -1;
    }

    for (i = 0; i < order_count; i++) {
        if (order[i] >= tuple_count) {
            free(temp);
            temp = NULL;
            return -1;
        }
        temp[i] = tuples[order[i]];
    }

    for (i = 0; i < order_count; i++) {
        tuples[i] = temp[i];
    }

    free(temp);
    temp = NULL;

    return 0;
}

void print_tuples(const Tuple *tuples, size_t count)
{
    size_t i;

    if (tuples == NULL || count == 0) {
        return;
    }

    for (i = 0; i < count; i++) {
        printf("Tuple %zu: id=%d, name=%.*s, value=%.2f\n",
               i, tuples[i].id, (int)(NAME_SIZE - 1), tuples[i].name, tuples[i].value);
    }
}

int main(void)
{
    Tuple tuples[5];
    size_t order[] = {2, 0, 4, 1, 3};
    size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    size_t order_count = sizeof(order) / sizeof(order[0]);
    int result;

    if (tuple_init(&tuples[0], 1, "Alpha", 10.5f) != 0 ||
        tuple_init(&tuples[1], 2, "Beta", 20.3f) != 0 ||
        tuple_init(&tuples[2], 3, "Gamma", 30.7f) != 0 ||
        tuple_init(&tuples[3], 4, "Delta", 40.1f) != 0 ||
        tuple_init(&tuples[4], 5, "Epsilon", 50.9f) != 0) {
        fprintf(stderr, "Error: Failed to initialize tuples\n");
        return EXIT_FAILURE;
    }

    printf("Original tuples:\n");
    print_tuples(tuples, tuple_count);

    result = rearrange_tuples(tuples, tuple_count, order, order_count);

    if (result != 0) {
        fprintf(stderr, "Error: Failed to rearrange tuples\n");
        return EXIT_FAILURE;
    }

    printf("\nRearranged tuples:\n");
    print_tuples(tuples, tuple_count);

    return EXIT_SUCCESS;
}