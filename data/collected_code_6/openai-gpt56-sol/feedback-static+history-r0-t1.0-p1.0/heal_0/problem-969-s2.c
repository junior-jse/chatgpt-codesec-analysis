#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct {
    int first;
    int *values;
    size_t value_count;
    size_t value_capacity;
} JoinedTuple;

static void free_joined_tuples(JoinedTuple *tuples, size_t count)
{
    if (tuples == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(tuples[i].values);
    }

    free(tuples);
}

static int append_value(JoinedTuple *tuple, int value)
{
    if (tuple == NULL) {
        return 0;
    }

    if (tuple->value_count == tuple->value_capacity) {
        size_t new_capacity = tuple->value_capacity == 0U ? 2U : tuple->value_capacity * 2U;

        if (new_capacity < tuple->value_capacity ||
            new_capacity > SIZE_MAX / sizeof(*tuple->values)) {
            return 0;
        }

        int *new_values = realloc(tuple->values,
                                  new_capacity * sizeof(*tuple->values));
        if (new_values == NULL) {
            return 0;
        }

        tuple->values = new_values;
        tuple->value_capacity = new_capacity;
    }

    tuple->values[tuple->value_count++] = value;
    return 1;
}

static int join_tuples(const Tuple *input, size_t input_count,
                       JoinedTuple **output, size_t *output_count)
{
    JoinedTuple *joined = NULL;
    size_t joined_count = 0U;

    if (output == NULL || output_count == NULL ||
        (input == NULL && input_count != 0U)) {
        return 0;
    }

    *output = NULL;
    *output_count = 0U;

    if (input_count == 0U) {
        return 1;
    }

    if (input_count > SIZE_MAX / sizeof(*joined)) {
        return 0;
    }

    joined = calloc(input_count, sizeof(*joined));
    if (joined == NULL) {
        return 0;
    }

    for (size_t i = 0; i < input_count; ++i) {
        size_t group = 0U;

        while (group < joined_count && joined[group].first != input[i].first) {
            ++group;
        }

        if (group == joined_count) {
            joined[group].first = input[i].first;
            ++joined_count;
        }

        if (!append_value(&joined[group], input[i].second)) {
            free_joined_tuples(joined, joined_count);
            return 0;
        }
    }

    *output = joined;
    *output_count = joined_count;
    return 1;
}

int main(void)
{
    const Tuple tuples[] = {
        {1, 2},
        {1, 3},
        {2, 4},
        {3, 5},
        {2, 6},
        {1, 7}
    };
    JoinedTuple *joined = NULL;
    size_t joined_count = 0U;
    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);

    if (!join_tuples(tuples, tuple_count, &joined, &joined_count)) {
        fputs("Failed to join tuples.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < joined_count; ++i) {
        printf("(%d", joined[i].first);

        for (size_t j = 0; j < joined[i].value_count; ++j) {
            printf(", %d", joined[i].values[j]);
        }

        puts(")");
    }

    free_joined_tuples(joined, joined_count);
    return EXIT_SUCCESS;
}