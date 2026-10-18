#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    const char *first;
    const char *second;
} Tuple;

static int checked_add(size_t a, size_t b, size_t *result)
{
    if (a > SIZE_MAX - b) {
        return 0;
    }

    *result = a + b;
    return 1;
}

static char *flatten_tuple_list(const Tuple *tuples, size_t count)
{
    size_t length = 0;
    size_t offset = 0;
    char *result;

    if (count > 0 && tuples == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        size_t first_length;
        size_t second_length;

        if (tuples[i].first == NULL || tuples[i].second == NULL) {
            return NULL;
        }

        first_length = strlen(tuples[i].first);
        second_length = strlen(tuples[i].second);

        if (!checked_add(length, first_length, &length) ||
            !checked_add(length, 1, &length) ||
            !checked_add(length, second_length, &length) ||
            (i + 1 < count && !checked_add(length, 1, &length))) {
            return NULL;
        }
    }

    if (!checked_add(length, 1, &length)) {
        return NULL;
    }

    result = malloc(length);
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        size_t first_length = strlen(tuples[i].first);
        size_t second_length = strlen(tuples[i].second);

        memcpy(result + offset, tuples[i].first, first_length);
        offset += first_length;
        result[offset++] = ',';
        memcpy(result + offset, tuples[i].second, second_length);
        offset += second_length;

        if (i + 1 < count) {
            result[offset++] = ' ';
        }
    }

    result[offset] = '\0';
    return result;
}

int main(void)
{
    const Tuple tuples[] = {
        {"apple", "red"},
        {"banana", "yellow"},
        {"grape", "purple"}
    };
    const size_t count = sizeof(tuples) / sizeof(tuples[0]);
    char *flattened = flatten_tuple_list(tuples, count);

    if (flattened == NULL) {
        fputs("Failed to flatten tuple list.\n", stderr);
        return EXIT_FAILURE;
    }

    if (puts(flattened) == EOF) {
        free(flattened);
        return EXIT_FAILURE;
    }

    free(flattened);
    return EXIT_SUCCESS;
}