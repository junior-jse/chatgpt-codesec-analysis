#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *values;
    bool *is_none;
    size_t length;
} Tuple;

static bool tuple_is_all_none(const Tuple *tuple)
{
    if (tuple == NULL) {
        return false;
    }

    for (size_t i = 0; i < tuple->length; ++i) {
        if (tuple->is_none == NULL || !tuple->is_none[i]) {
            return false;
        }
    }

    return true;
}

static void free_tuple(Tuple *tuple)
{
    if (tuple == NULL) {
        return;
    }

    free(tuple->values);
    free(tuple->is_none);
    tuple->values = NULL;
    tuple->is_none = NULL;
    tuple->length = 0;
}

static size_t remove_all_none_tuples(Tuple *tuples, size_t count)
{
    if (tuples == NULL) {
        return 0;
    }

    size_t retained = 0;

    for (size_t i = 0; i < count; ++i) {
        if (tuple_is_all_none(&tuples[i])) {
            free_tuple(&tuples[i]);
        } else {
            if (retained != i) {
                tuples[retained] = tuples[i];
                tuples[i].values = NULL;
                tuples[i].is_none = NULL;
                tuples[i].length = 0;
            }
            ++retained;
        }
    }

    return retained;
}

static bool initialize_tuple(Tuple *tuple, size_t length)
{
    if (tuple == NULL || length == 0) {
        return false;
    }

    tuple->values = calloc(length, sizeof(*tuple->values));
    tuple->is_none = calloc(length, sizeof(*tuple->is_none));
    tuple->length = 0;

    if (tuple->values == NULL || tuple->is_none == NULL) {
        free(tuple->values);
        free(tuple->is_none);
        tuple->values = NULL;
        tuple->is_none = NULL;
        return false;
    }

    tuple->length = length;
    return true;
}

int main(void)
{
    const size_t capacity = 4;
    Tuple *tuples = calloc(capacity, sizeof(*tuples));

    if (tuples == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < capacity; ++i) {
        if (!initialize_tuple(&tuples[i], 3)) {
            for (size_t j = 0; j < i; ++j) {
                free_tuple(&tuples[j]);
            }
            free(tuples);
            return EXIT_FAILURE;
        }
    }

    tuples[0].is_none[0] = true;
    tuples[0].is_none[1] = true;
    tuples[0].is_none[2] = true;

    tuples[1].values[0] = 10;
    tuples[1].is_none[1] = true;
    tuples[1].values[2] = 30;

    tuples[2].is_none[0] = true;
    tuples[2].is_none[1] = true;
    tuples[2].is_none[2] = true;

    tuples[3].values[0] = 40;
    tuples[3].values[1] = 50;
    tuples[3].is_none[2] = true;

    size_t count = remove_all_none_tuples(tuples, capacity);

    for (size_t i = 0; i < count; ++i) {
        printf("(");
        for (size_t j = 0; j < tuples[i].length; ++j) {
            if (tuples[i].is_none[j]) {
                printf("None");
            } else {
                printf("%d", tuples[i].values[j]);
            }
            printf("%s", j + 1 < tuples[i].length ? ", " : "");
        }
        printf(")\n");
    }

    for (size_t i = 0; i < count; ++i) {
        free_tuple(&tuples[i]);
    }

    free(tuples);
    return EXIT_SUCCESS;
}