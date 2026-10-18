#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *items;
    size_t length;
} Tuple;

static int trim_tuples(Tuple *tuples, size_t count, size_t k)
{
    if (tuples == NULL && count != 0) {
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        if (tuples[i].items == NULL && tuples[i].length != 0) {
            return -1;
        }

        if (k >= tuples[i].length) {
            free(tuples[i].items);
            tuples[i].items = NULL;
            tuples[i].length = 0;
            continue;
        }

        size_t new_length = tuples[i].length - k;
        int *resized = realloc(tuples[i].items, new_length * sizeof(*resized));

        if (resized != NULL) {
            tuples[i].items = resized;
        }

        tuples[i].length = new_length;
    }

    return 0;
}

static void free_tuples(Tuple *tuples, size_t count)
{
    if (tuples == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(tuples[i].items);
    }

    free(tuples);
}

int main(void)
{
    const size_t lengths[] = {4, 3, 5};
    const int values[][5] = {
        {1, 2, 3, 4, 0},
        {5, 6, 7, 0, 0},
        {8, 9, 10, 11, 12}
    };
    const size_t count = sizeof(lengths) / sizeof(lengths[0]);
    const size_t k = 2;

    Tuple *tuples = calloc(count, sizeof(*tuples));
    if (tuples == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; ++i) {
        if (lengths[i] > SIZE_MAX / sizeof(*tuples[i].items)) {
            free_tuples(tuples, count);
            return EXIT_FAILURE;
        }

        tuples[i].items = malloc(lengths[i] * sizeof(*tuples[i].items));
        if (tuples[i].items == NULL) {
            free_tuples(tuples, count);
            return EXIT_FAILURE;
        }

        tuples[i].length = lengths[i];

        for (size_t j = 0; j < lengths[i]; ++j) {
            tuples[i].items[j] = values[i][j];
        }
    }

    if (trim_tuples(tuples, count, k) != 0) {
        free_tuples(tuples, count);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; ++i) {
        if (putchar('(') == EOF) {
            free_tuples(tuples, count);
            return EXIT_FAILURE;
        }

        for (size_t j = 0; j < tuples[i].length; ++j) {
            if (j != 0 && printf(", ") < 0) {
                free_tuples(tuples, count);
                return EXIT_FAILURE;
            }

            if (printf("%d", tuples[i].items[j]) < 0) {
                free_tuples(tuples, count);
                return EXIT_FAILURE;
            }
        }

        if (printf(")%c", i + 1 == count ? '\n' : ' ') < 0) {
            free_tuples(tuples, count);
            return EXIT_FAILURE;
        }
    }

    free_tuples(tuples, count);
    return EXIT_SUCCESS;
}