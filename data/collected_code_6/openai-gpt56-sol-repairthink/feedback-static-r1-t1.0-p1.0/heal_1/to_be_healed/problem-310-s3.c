#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
} Tuple;

static void tuple_destroy(Tuple *tuple)
{
    if (tuple == NULL) {
        return;
    }

    for (size_t i = 0; i < tuple->count; ++i) {
        free(tuple->items[i]);
    }

    free(tuple->items);
    tuple->items = NULL;
    tuple->count = 0;
}

static int tuple_append(Tuple *tuple, const char *start, size_t length)
{
    if (tuple == NULL || start == NULL || length == SIZE_MAX) {
        return -1;
    }

    if (tuple->count == SIZE_MAX / sizeof(*tuple->items)) {
        return -1;
    }

    size_t new_count = tuple->count + 1;
    char **new_items = realloc(tuple->items, new_count * sizeof(*new_items));
    if (new_items == NULL) {
        return -1;
    }

    tuple->items = new_items;
    tuple->items[tuple->count] = malloc(length + 1);
    if (tuple->items[tuple->count] == NULL) {
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(tuple->items[tuple->count], start, length);
    tuple->items[tuple->count][length] = '\0';
    tuple->count = new_count;

    return 0;
}

static int string_to_tuple(const char *input, char delimiter, Tuple *tuple)
{
    if (input == NULL || tuple == NULL) {
        return -1;
    }

    tuple->items = NULL;
    tuple->count = 0;

    const char *start = input;
    const char *cursor = input;

    for (;;) {
        if (*cursor == delimiter || *cursor == '\0') {
            size_t length = (size_t)(cursor - start);

            if (tuple_append(tuple, start, length) != 0) {
                tuple_destroy(tuple);
                return -1;
            }

            if (*cursor == '\0') {
                break;
            }

            start = cursor + 1;
        }

        ++cursor;
    }

    return 0;
}

static void tuple_print(const Tuple *tuple)
{
    if (tuple == NULL) {
        return;
    }

    putchar('(');

    for (size_t i = 0; i < tuple->count; ++i) {
        printf("\"%s\"%s", tuple->items[i], i + 1 < tuple->count ? ", " : "");
    }

    puts(")");
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <comma-separated-string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Tuple tuple;

    if (string_to_tuple(argv[1], ',', &tuple) != 0) {
        fputs("Failed to convert string to tuple\n", stderr);
        return EXIT_FAILURE;
    }

    tuple_print(&tuple);
    tuple_destroy(&tuple);

    return EXIT_SUCCESS;
}