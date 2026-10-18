#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *initial;
    char *value;
} Tuple;

typedef struct {
    char *initial;
    char **values;
    size_t count;
    size_t capacity;
} JoinedTuple;

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source);
    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1U);
    return copy;
}

static void free_joined_tuples(JoinedTuple *tuples, size_t count)
{
    size_t i;
    size_t j;

    if (tuples == NULL) {
        return;
    }

    for (i = 0U; i < count; ++i) {
        free(tuples[i].initial);
        for (j = 0U; j < tuples[i].count; ++j) {
            free(tuples[i].values[j]);
        }
        free(tuples[i].values);
    }

    free(tuples);
}

static int append_value(JoinedTuple *tuple, const char *value)
{
    char *copy;

    if (tuple == NULL || value == NULL) {
        return -1;
    }

    if (tuple->count == tuple->capacity) {
        size_t new_capacity;
        char **new_values;

        if (tuple->capacity > SIZE_MAX / 2U) {
            return -1;
        }

        new_capacity = tuple->capacity == 0U ? 2U : tuple->capacity * 2U;
        if (new_capacity > SIZE_MAX / sizeof(*new_values)) {
            return -1;
        }

        new_values = realloc(tuple->values,
                             new_capacity * sizeof(*new_values));
        if (new_values == NULL) {
            return -1;
        }

        tuple->values = new_values;
        tuple->capacity = new_capacity;
    }

    copy = duplicate_string(value);
    if (copy == NULL) {
        return -1;
    }

    tuple->values[tuple->count] = copy;
    ++tuple->count;
    return 0;
}

static int join_tuples(const Tuple *input, size_t input_count,
                       JoinedTuple **output, size_t *output_count)
{
    JoinedTuple *result = NULL;
    size_t result_count = 0U;
    size_t result_capacity = 0U;
    size_t i;

    if (output == NULL || output_count == NULL ||
        (input == NULL && input_count != 0U)) {
        return -1;
    }

    *output = NULL;
    *output_count = 0U;

    for (i = 0U; i < input_count; ++i) {
        size_t group;

        if (input[i].initial == NULL || input[i].value == NULL) {
            free_joined_tuples(result, result_count);
            return -1;
        }

        for (group = 0U; group < result_count; ++group) {
            if (strcmp(result[group].initial, input[i].initial) == 0) {
                break;
            }
        }

        if (group == result_count) {
            JoinedTuple *new_result;

            if (result_count == result_capacity) {
                size_t new_capacity =
                    result_capacity == 0U ? 4U : result_capacity * 2U;

                if (result_capacity > SIZE_MAX / 2U ||
                    new_capacity > SIZE_MAX / sizeof(*new_result)) {
                    free_joined_tuples(result, result_count);
                    return -1;
                }

                new_result = realloc(
                    result, new_capacity * sizeof(*new_result));
                if (new_result == NULL) {
                    free_joined_tuples(result, result_count);
                    return -1;
                }

                result = new_result;
                result_capacity = new_capacity;
            }

            result[group].initial = duplicate_string(input[i].initial);
            if (result[group].initial == NULL) {
                free_joined_tuples(result, result_count);
                return -1;
            }

            result[group].values = NULL;
            result[group].count = 0U;
            result[group].capacity = 0U;
            ++result_count;
        }

        if (append_value(&result[group], input[i].value) != 0) {
            free_joined_tuples(result, result_count);
            return -1;
        }
    }

    *output = result;
    *output_count = result_count;
    return 0;
}

int main(void)
{
    const Tuple input[] = {
        {"A", "apple"},
        {"B", "banana"},
        {"A", "apricot"},
        {"C", "cherry"},
        {"B", "blueberry"}
    };
    JoinedTuple *joined = NULL;
    size_t joined_count = 0U;
    size_t i;
    size_t j;

    if (join_tuples(input, sizeof(input) / sizeof(input[0]),
                    &joined, &joined_count) != 0) {
        fputs("Failed to join tuples.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < joined_count; ++i) {
        if (printf("(%s", joined[i].initial) < 0) {
            free_joined_tuples(joined, joined_count);
            return EXIT_FAILURE;
        }

        for (j = 0U; j < joined[i].count; ++j) {
            if (printf(", %s", joined[i].values[j]) < 0) {
                free_joined_tuples(joined, joined_count);
                return EXIT_FAILURE;
            }
        }

        if (puts(")") == EOF) {
            free_joined_tuples(joined, joined_count);
            return EXIT_FAILURE;
        }
    }

    free_joined_tuples(joined, joined_count);
    return EXIT_SUCCESS;
}