#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TUPLE_NAME_SIZE 50

typedef struct {
    int id;
    char name[TUPLE_NAME_SIZE];
    int value;
} Tuple;

static size_t tuple_bounded_len(const char *s, size_t max_len)
{
    size_t len = 0;

    while (len < max_len && s[len] != '\0') {
        len++;
    }

    return len;
}

static int tuple_set_name(Tuple *tuple, const char *name)
{
    size_t name_len;
    size_t i;

    if (tuple == NULL || name == NULL) {
        return -1;
    }

    name_len = tuple_bounded_len(name, TUPLE_NAME_SIZE);
    if (name_len >= TUPLE_NAME_SIZE) {
        tuple->name[0] = '\0';
        return -1;
    }

    for (i = 0; i < name_len; i++) {
        tuple->name[i] = name[i];
    }
    tuple->name[name_len] = '\0';

    return 0;
}

static int tuple_init(Tuple *tuple, int id, const char *name, int value)
{
    if (tuple == NULL) {
        return -1;
    }

    tuple->id = id;
    tuple->value = value;
    tuple->name[0] = '\0';

    if (tuple_set_name(tuple, name) != 0) {
        return -1;
    }

    return 0;
}

int rearrange_tuples(Tuple *tuples, size_t tuple_count, const size_t *order, size_t order_count)
{
    Tuple *temp;
    size_t i;
    size_t bytes;

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

    for (i = 0; i < order_count; i++) {
        if (order[i] >= tuple_count) {
            return -1;
        }
    }

    bytes = tuple_count * sizeof(Tuple);

    temp = (Tuple *)malloc(bytes);
    if (temp == NULL) {
        return -1;
    }

    for (i = 0; i < tuple_count; i++) {
        temp[i] = tuples[i];
    }

    for (i = 0; i < order_count; i++) {
        tuples[i] = temp[order[i]];
    }

    free(temp);
    temp = NULL;

    return 0;
}

void print_tuples(const Tuple *tuples, size_t count)
{
    size_t i;

    if (tuples == NULL) {
        return;
    }

    for (i = 0; i < count; i++) {
        printf("Tuple %zu: id=%d, name=%.*s, value=%d\n",
               i, tuples[i].id, TUPLE_NAME_SIZE - 1, tuples[i].name, tuples[i].value);
    }
}

int main(void)
{
    Tuple tuples[5];
    size_t order[] = {2, 0, 4, 1, 3};
    size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    size_t order_count = sizeof(order) / sizeof(order[0]);
    int result;

    if (tuple_init(&tuples[0], 1, "Alpha", 100) != 0 ||
        tuple_init(&tuples[1], 2, "Beta", 200) != 0 ||
        tuple_init(&tuples[2], 3, "Gamma", 300) != 0 ||
        tuple_init(&tuples[3], 4, "Delta", 400) != 0 ||
        tuple_init(&tuples[4], 5, "Epsilon", 500) != 0) {
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