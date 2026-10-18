#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *items;
    size_t size;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t size;
} TupleList;

static void free_tuple_list(TupleList *list)
{
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < list->size; ++i) {
        free(list->tuples[i].items);
        list->tuples[i].items = NULL;
        list->tuples[i].size = 0;
    }

    free(list->tuples);
    list->tuples = NULL;
    list->size = 0;
}

static int remove_empty_tuples(TupleList *list)
{
    if (list == NULL || (list->size > 0 && list->tuples == NULL)) {
        return -1;
    }

    size_t write_index = 0;

    for (size_t read_index = 0; read_index < list->size; ++read_index) {
        if (list->tuples[read_index].size == 0) {
            free(list->tuples[read_index].items);
            continue;
        }

        if (list->tuples[read_index].items == NULL) {
            return -1;
        }

        if (write_index != read_index) {
            list->tuples[write_index] = list->tuples[read_index];
        }
        ++write_index;
    }

    if (write_index == 0) {
        free(list->tuples);
        list->tuples = NULL;
    } else {
        Tuple *resized = realloc(list->tuples, write_index * sizeof(*resized));
        if (resized != NULL) {
            list->tuples = resized;
        }
    }

    list->size = write_index;
    return 0;
}

static int initialize_tuple(Tuple *tuple, const int *values, size_t size)
{
    if (tuple == NULL || (size > 0 && values == NULL)) {
        return -1;
    }

    tuple->items = NULL;
    tuple->size = 0;

    if (size == 0) {
        return 0;
    }

    if (size > SIZE_MAX / sizeof(*tuple->items)) {
        return -1;
    }

    tuple->items = malloc(size * sizeof(*tuple->items));
    if (tuple->items == NULL) {
        return -1;
    }

    memcpy(tuple->items, values, size * sizeof(*tuple->items));
    tuple->size = size;
    return 0;
}

static void print_tuple_list(const TupleList *list)
{
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < list->size; ++i) {
        putchar('(');
        for (size_t j = 0; j < list->tuples[i].size; ++j) {
            if (j > 0) {
                printf(", ");
            }
            printf("%d", list->tuples[i].items[j]);
        }
        puts(")");
    }
}

int main(void)
{
    const int first[] = {1, 2};
    const int third[] = {3, 4, 5};
    TupleList list = {NULL, 0};

    list.size = 4;
    list.tuples = calloc(list.size, sizeof(*list.tuples));
    if (list.tuples == NULL) {
        fprintf(stderr, "Failed to allocate tuple list.\n");
        return EXIT_FAILURE;
    }

    if (initialize_tuple(&list.tuples[0], first, 2) != 0 ||
        initialize_tuple(&list.tuples[1], NULL, 0) != 0 ||
        initialize_tuple(&list.tuples[2], third, 3) != 0 ||
        initialize_tuple(&list.tuples[3], NULL, 0) != 0) {
        fprintf(stderr, "Failed to initialize tuples.\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    if (remove_empty_tuples(&list) != 0) {
        fprintf(stderr, "Failed to remove empty tuples.\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    print_tuple_list(&list);
    free_tuple_list(&list);
    return EXIT_SUCCESS;
}