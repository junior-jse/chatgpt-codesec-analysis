#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
} StringTuple;

static void free_tuple(StringTuple *tuple)
{
    size_t i;

    if (tuple == NULL) {
        return;
    }

    for (i = 0; i < tuple->count; ++i) {
        free(tuple->items[i]);
    }

    free(tuple->items);
    tuple->items = NULL;
    tuple->count = 0;
}

static char *duplicate_range(const char *start, size_t length)
{
    char *copy;

    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, start, length);
    copy[length] = '\0';
    return copy;
}

static int string_to_tuple(const char *input, char delimiter, StringTuple *tuple)
{
    const char *start;
    const char *current;
    size_t count = 1;
    size_t index = 0;
    char **items;

    if (input == NULL || tuple == NULL || delimiter == '\0') {
        return -1;
    }

    tuple->items = NULL;
    tuple->count = 0;

    for (current = input; *current != '\0'; ++current) {
        if (*current == delimiter) {
            if (count == SIZE_MAX) {
                return -1;
            }
            ++count;
        }
    }

    if (count > SIZE_MAX / sizeof(*items)) {
        return -1;
    }

    items = calloc(count, sizeof(*items));
    if (items == NULL) {
        return -1;
    }

    start = input;
    for (current = input;; ++current) {
        if (*current == delimiter || *current == '\0') {
            items[index] = duplicate_range(start, (size_t)(current - start));
            if (items[index] == NULL) {
                tuple->items = items;
                tuple->count = index;
                free_tuple(tuple);
                return -1;
            }

            ++index;

            if (*current == '\0') {
                break;
            }

            start = current + 1;
        }
    }

    tuple->items = items;
    tuple->count = index;
    return 0;
}

static void print_tuple(const StringTuple *tuple)
{
    size_t i;

    putchar('(');

    for (i = 0; i < tuple->count; ++i) {
        if (i != 0) {
            fputs(", ", stdout);
        }
        printf("\"%s\"", tuple->items[i]);
    }

    if (tuple->count == 1) {
        putchar(',');
    }

    puts(")");
}

int main(int argc, char *argv[])
{
    StringTuple tuple = {NULL, 0};

    if (argc != 3 || argv[2][0] == '\0' || argv[2][1] != '\0') {
        fprintf(stderr, "Usage: %s <string> <delimiter>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (string_to_tuple(argv[1], argv[2][0], &tuple) != 0) {
        fputs("Failed to convert string to tuple.\n", stderr);
        return EXIT_FAILURE;
    }

    print_tuple(&tuple);
    free_tuple(&tuple);
    return EXIT_SUCCESS;
}