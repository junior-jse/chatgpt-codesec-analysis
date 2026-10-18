#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *items;
    size_t length;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t length;
} TupleList;

static void free_tuple_list(TupleList *list)
{
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < list->length; ++i) {
        free(list->tuples[i].items);
        list->tuples[i].items = NULL;
        list->tuples[i].length = 0;
    }

    free(list->tuples);
    list->tuples = NULL;
    list->length = 0;
}

static void remove_empty_tuples(TupleList *list)
{
    if (list == NULL || list->tuples == NULL) {
        return;
    }

    size_t output = 0;

    for (size_t input = 0; input < list->length; ++input) {
        if (list->tuples[input].length == 0) {
            free(list->tuples[input].items);
            continue;
        }

        if (output != input) {
            list->tuples[output] = list->tuples[input];
        }

        ++output;
    }

    list->length = output;

    if (output == 0) {
        free(list->tuples);
        list->tuples = NULL;
        return;
    }

    Tuple *resized = realloc(list->tuples, output * sizeof(*resized));
    if (resized != NULL) {
        list->tuples = resized;
    }
}

static int print_tuple_list(const TupleList *list)
{
    if (list == NULL || (list->length > 0 && list->tuples == NULL)) {
        return -1;
    }

    putchar('[');

    for (size_t i = 0; i < list->length; ++i) {
        putchar('(');

        for (size_t j = 0; j < list->tuples[i].length; ++j) {
            if (j > 0) {
                fputs(", ", stdout);
            }

            if (printf("%d", list->tuples[i].items[j]) < 0) {
                return -1;
            }
        }

        putchar(')');

        if (i + 1 < list->length) {
            fputs(", ", stdout);
        }
    }

    if (puts("]") == EOF) {
        return -1;
    }

    return ferror(stdout) ? -1 : 0;
}

int main(void)
{
    TupleList list = {0};
    const size_t lengths[] = {2, 0, 3, 0};
    const int values[][3] = {
        {1, 2, 0},
        {0, 0, 0},
        {3, 4, 5},
        {0, 0, 0}
    };

    list.length = sizeof(lengths) / sizeof(lengths[0]);
    list.tuples = calloc(list.length, sizeof(*list.tuples));

    if (list.tuples == NULL) {
        fputs("Failed to allocate tuple list.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < list.length; ++i) {
        list.tuples[i].length = lengths[i];

        if (lengths[i] == 0) {
            continue;
        }

        list.tuples[i].items = malloc(lengths[i] * sizeof(*list.tuples[i].items));

        if (list.tuples[i].items == NULL) {
            fputs("Failed to allocate tuple.\n", stderr);
            free_tuple_list(&list);
            return EXIT_FAILURE;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(list.tuples[i].items, values[i],
               lengths[i] * sizeof(*list.tuples[i].items));
    }

    remove_empty_tuples(&list);

    if (print_tuple_list(&list) != 0) {
        fputs("Failed to print tuple list.\n", stderr);
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    free_tuple_list(&list);
    return EXIT_SUCCESS;
}