#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    size_t size;
} Tuple;

static void free_tuples(Tuple *tuples, size_t count)
{
    size_t i;

    if (tuples == NULL) {
        return;
    }

    for (i = 0; i < count; ++i) {
        free(tuples[i].data);
        tuples[i].data = NULL;
        tuples[i].size = 0;
    }

    free(tuples);
}

static int trim_tuples(Tuple *tuples, size_t count, size_t k)
{
    size_t i;
    int *new_data;

    if (tuples == NULL) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        if (tuples[i].size > 0 && tuples[i].data == NULL) {
            return -1;
        }

        if (tuples[i].size <= k) {
            free(tuples[i].data);
            tuples[i].data = NULL;
            tuples[i].size = 0;
            continue;
        }

        new_data = realloc(tuples[i].data, (tuples[i].size - k) * sizeof(int));
        if (new_data == NULL) {
            return -1;
        }

        tuples[i].data = new_data;
        tuples[i].size -= k;
    }

    return 0;
}

static void print_tuples(const Tuple *tuples, size_t count)
{
    size_t i;
    size_t j;

    if (tuples == NULL) {
        return;
    }

    for (i = 0; i < count; ++i) {
        printf("(");
        for (j = 0; j < tuples[i].size; ++j) {
            printf("%d", tuples[i].data[j]);
            if (j + 1 < tuples[i].size) {
                printf(", ");
            }
        }
        printf(")\n");
    }
}

int main(void)
{
    Tuple *tuples;
    size_t count = 3;
    size_t k = 1;
    size_t i;
    size_t j;
    int values[][5] = {
        {1, 2, 3, 4, 5},
        {10, 20, 30, 0, 0},
        {7, 8, 9, 10, 11}
    };
    size_t sizes[] = {5, 3, 5};

    tuples = calloc(count, sizeof(Tuple));
    if (tuples == NULL) {
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        tuples[i].data = malloc(sizes[i] * sizeof(int));
        if (tuples[i].data == NULL) {
            free_tuples(tuples, count);
            return EXIT_FAILURE;
        }

        tuples[i].size = sizes[i];

        for (j = 0; j < sizes[i]; ++j) {
            tuples[i].data[j] = values[i][j];
        }
    }

    if (trim_tuples(tuples, count, k) != 0) {
        free_tuples(tuples, count);
        return EXIT_FAILURE;
    }

    print_tuples(tuples, count);
    free_tuples(tuples, count);

    return EXIT_SUCCESS;
}