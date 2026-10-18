#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int *values;
    size_t length;
} Tuple;

static int trim_tuples(const Tuple *input, size_t count, size_t k, Tuple **output)
{
    Tuple *result;
    size_t i;

    if (output == NULL || (count > 0 && input == NULL)) {
        return -1;
    }

    *output = NULL;

    if (count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = calloc(count, sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        size_t trimmed_length;

        if (input[i].length > 0 && input[i].values == NULL) {
            goto failure;
        }

        trimmed_length = input[i].length > k ? input[i].length - k : 0;
        result[i].length = trimmed_length;

        if (trimmed_length == 0) {
            continue;
        }

        if (trimmed_length > SIZE_MAX / sizeof(*result[i].values)) {
            goto failure;
        }

        result[i].values = malloc(trimmed_length * sizeof(*result[i].values));
        if (result[i].values == NULL) {
            goto failure;
        }

        for (size_t j = 0; j < trimmed_length; ++j) {
            result[i].values[j] = input[i].values[j];
        }
    }

    *output = result;
    return 0;

failure:
    for (size_t j = 0; j < count; ++j) {
        free(result[j].values);
    }
    free(result);
    return -1;
}

static void free_tuples(Tuple *tuples, size_t count)
{
    if (tuples == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(tuples[i].values);
    }

    free(tuples);
}

static int print_tuples(const Tuple *tuples, size_t count)
{
    if (count > 0 && tuples == NULL) {
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        if (printf("(") < 0) {
            return -1;
        }

        for (size_t j = 0; j < tuples[i].length; ++j) {
            if (printf("%s%d", j == 0 ? "" : ", ", tuples[i].values[j]) < 0) {
                return -1;
            }
        }

        if (printf(")%s", i + 1 == count ? "\n" : ", ") < 0) {
            return -1;
        }
    }

    return 0;
}

int main(void)
{
    int first[] = {1, 2, 3, 4};
    int second[] = {5, 6, 7};
    int third[] = {8, 9};
    Tuple input[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
    };
    const size_t count = sizeof(input) / sizeof(input[0]);
    const size_t k = 1;
    Tuple *trimmed = NULL;
    int status = EXIT_FAILURE;

    if (trim_tuples(input, count, k, &trimmed) != 0) {
        fputs("Failed to trim tuples.\n", stderr);
        return EXIT_FAILURE;
    }

    if (print_tuples(trimmed, count) == 0) {
        status = EXIT_SUCCESS;
    } else {
        fputs("Failed to print tuples.\n", stderr);
    }

    free_tuples(trimmed, count);
    return status;
}