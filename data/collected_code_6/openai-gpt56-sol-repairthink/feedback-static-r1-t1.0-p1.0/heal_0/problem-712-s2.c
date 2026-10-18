#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int *values;
    size_t length;
} IntList;

static bool lists_equal(const IntList *a, const IntList *b)
{
    size_t i;

    if (a->length != b->length) {
        return false;
    }

    for (i = 0; i < a->length; ++i) {
        if (a->values[i] != b->values[i]) {
            return false;
        }
    }

    return true;
}

static void free_list(IntList *list)
{
    free(list->values);
    list->values = NULL;
    list->length = 0;
}

static void free_lists(IntList *lists, size_t count)
{
    size_t i;

    if (lists == NULL) {
        return;
    }

    for (i = 0; i < count; ++i) {
        free_list(&lists[i]);
    }

    free(lists);
}

static void remove_duplicate_lists(IntList *lists, size_t *count)
{
    size_t read_index;
    size_t write_index;

    if (lists == NULL || count == NULL) {
        return;
    }

    write_index = 0;

    for (read_index = 0; read_index < *count; ++read_index) {
        size_t i;
        bool duplicate = false;

        for (i = 0; i < write_index; ++i) {
            if (lists_equal(&lists[read_index], &lists[i])) {
                duplicate = true;
                break;
            }
        }

        if (duplicate) {
            free_list(&lists[read_index]);
        } else {
            if (write_index != read_index) {
                lists[write_index] = lists[read_index];
                lists[read_index].values = NULL;
                lists[read_index].length = 0;
            }
            ++write_index;
        }
    }

    *count = write_index;
}

static bool initialize_list(IntList *list, const int *values, size_t length)
{
    size_t i;

    if (list == NULL || (values == NULL && length != 0)) {
        return false;
    }

    list->values = NULL;
    list->length = 0;

    if (length == 0) {
        return true;
    }

    if (length > SIZE_MAX / sizeof(*list->values)) {
        return false;
    }

    list->values = malloc(length * sizeof(*list->values));
    if (list->values == NULL) {
        return false;
    }

    for (i = 0; i < length; ++i) {
        list->values[i] = values[i];
    }

    list->length = length;
    return true;
}

static void print_lists(const IntList *lists, size_t count)
{
    size_t i;

    for (i = 0; i < count; ++i) {
        size_t j;

        putchar('[');
        for (j = 0; j < lists[i].length; ++j) {
            if (j != 0) {
                fputs(", ", stdout);
            }
            printf("%d", lists[i].values[j]);
        }
        puts("]");
    }
}

int main(void)
{
    static const int data0[] = {1, 2, 3};
    static const int data1[] = {4, 5};
    static const int data2[] = {1, 2, 3};
    static const int data3[] = {6};
    static const int data4[] = {4, 5};
    static const int *const data[] = {
        data0, data1, data2, data3, data4
    };
    static const size_t lengths[] = {3, 2, 3, 1, 2};
    const size_t initial_count = sizeof(data) / sizeof(data[0]);
    IntList *lists;
    size_t count;
    size_t i;

    if (initial_count > SIZE_MAX / sizeof(*lists)) {
        fputs("Input is too large.\n", stderr);
        return EXIT_FAILURE;
    }

    lists = calloc(initial_count, sizeof(*lists));
    if (lists == NULL) {
        fputs("Memory allocation failed.\n", stderr);
        return EXIT_FAILURE;
    }

    count = initial_count;

    for (i = 0; i < count; ++i) {
        if (!initialize_list(&lists[i], data[i], lengths[i])) {
            fputs("Memory allocation failed.\n", stderr);
            free_lists(lists, count);
            return EXIT_FAILURE;
        }
    }

    remove_duplicate_lists(lists, &count);
    print_lists(lists, count);
    free_lists(lists, count);

    return EXIT_SUCCESS;
}