#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} Tuple;

static void free_tuples(Tuple *tuples, size_t count)
{
    if (tuples == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(tuples[i].key);
        free(tuples[i].value);
    }

    free(tuples);
}

static int rearrange_tuples(const Tuple *tuples,
                            size_t tuple_count,
                            const char *const *ordered_keys,
                            size_t key_count,
                            Tuple **result)
{
    Tuple *arranged = NULL;
    unsigned char *used = NULL;
    size_t output_index = 0;

    if (result == NULL ||
        (tuple_count > 0 && tuples == NULL) ||
        (key_count > 0 && ordered_keys == NULL)) {
        return -1;
    }

    *result = NULL;

    if (tuple_count == 0) {
        return 0;
    }

    if (tuple_count > SIZE_MAX / sizeof(*arranged)) {
        return -1;
    }

    arranged = calloc(tuple_count, sizeof(*arranged));
    used = calloc(tuple_count, sizeof(*used));

    if (arranged == NULL || used == NULL) {
        free(arranged);
        free(used);
        return -1;
    }

    for (size_t i = 0; i < key_count; ++i) {
        if (ordered_keys[i] == NULL) {
            free(arranged);
            free(used);
            return -1;
        }

        for (size_t j = 0; j < tuple_count; ++j) {
            if (!used[j] &&
                tuples[j].key != NULL &&
                strcmp(tuples[j].key, ordered_keys[i]) == 0) {
                arranged[output_index++] = tuples[j];
                used[j] = 1;
            }
        }
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        if (!used[i]) {
            arranged[output_index++] = tuples[i];
        }
    }

    free(used);
    *result = arranged;
    return 0;
}

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source);

    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1);

    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1);
    return copy;
}

int main(void)
{
    const char *input[][2] = {
        {"name", "Alice"},
        {"country", "Canada"},
        {"age", "30"},
        {"email", "alice@example.com"}
    };
    const char *order[] = {"age", "name", "email"};
    const size_t tuple_count = sizeof(input) / sizeof(input[0]);
    const size_t order_count = sizeof(order) / sizeof(order[0]);
    Tuple *tuples = NULL;
    Tuple *arranged = NULL;
    size_t initialized = 0;
    int status = EXIT_FAILURE;

    tuples = calloc(tuple_count, sizeof(*tuples));

    if (tuples == NULL) {
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        tuples[i].key = duplicate_string(input[i][0]);
        tuples[i].value = duplicate_string(input[i][1]);

        if (tuples[i].key == NULL || tuples[i].value == NULL) {
            initialized = i + 1;
            fputs("Memory allocation failed\n", stderr);
            goto cleanup;
        }

        initialized = i + 1;
    }

    if (rearrange_tuples(tuples,
                         tuple_count,
                         order,
                         order_count,
                         &arranged) != 0) {
        fputs("Unable to rearrange tuples\n", stderr);
        goto cleanup;
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        if (printf("(%s, %s)\n", arranged[i].key, arranged[i].value) < 0) {
            fputs("Output failed\n", stderr);
            goto cleanup;
        }
    }

    status = EXIT_SUCCESS;

cleanup:
    free(arranged);
    free_tuples(tuples, initialized);
    return status;
}