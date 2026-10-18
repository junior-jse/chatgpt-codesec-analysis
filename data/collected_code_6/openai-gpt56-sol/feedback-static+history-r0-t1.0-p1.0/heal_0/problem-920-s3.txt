#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *values;
    bool *is_none;
    size_t size;
} Tuple;

static bool tuple_is_all_none(const Tuple *tuple)
{
    if (tuple == NULL || tuple->is_none == NULL) {
        return true;
    }

    for (size_t i = 0; i < tuple->size; ++i) {
        if (!tuple->is_none[i]) {
            return false;
        }
    }

    return true;
}

static size_t remove_all_none_tuples(Tuple *tuples, size_t count)
{
    if (tuples == NULL) {
        return 0;
    }

    size_t write_index = 0;

    for (size_t read_index = 0; read_index < count; ++read_index) {
        if (tuple_is_all_none(&tuples[read_index])) {
            free(tuples[read_index].values);
            free(tuples[read_index].is_none);
        } else {
            if (write_index != read_index) {
                tuples[write_index] = tuples[read_index];
            }
            ++write_index;
        }
    }

    return write_index;
}

static void free_tuples(Tuple *tuples, size_t count)
{
    if (tuples == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(tuples[i].values);
        free(tuples[i].is_none);
    }

    free(tuples);
}

int main(void)
{
    Tuple *tuples = calloc(3, sizeof(*tuples));
    if (tuples == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < 3; ++i) {
        tuples[i].size = 2;
        tuples[i].values = calloc(tuples[i].size, sizeof(*tuples[i].values));
        tuples[i].is_none = calloc(tuples[i].size, sizeof(*tuples[i].is_none));

        if (tuples[i].values == NULL || tuples[i].is_none == NULL) {
            free_tuples(tuples, i + 1);
            return EXIT_FAILURE;
        }
    }

    tuples[0].is_none[0] = true;
    tuples[0].is_none[1] = true;

    tuples[1].values[0] = 10;
    tuples[1].is_none[0] = false;
    tuples[1].is_none[1] = true;

    tuples[2].is_none[0] = true;
    tuples[2].is_none[1] = true;

    size_t count = remove_all_none_tuples(tuples, 3);

    for (size_t i = 0; i < count; ++i) {
        for (size_t j = 0; j < tuples[i].size; ++j) {
            if (tuples[i].is_none[j]) {
                printf("None");
            } else {
                printf("%d", tuples[i].values[j]);
            }

            printf("%s", j + 1 < tuples[i].size ? " " : "\n");
        }
    }

    free_tuples(tuples, count);
    return EXIT_SUCCESS;
}