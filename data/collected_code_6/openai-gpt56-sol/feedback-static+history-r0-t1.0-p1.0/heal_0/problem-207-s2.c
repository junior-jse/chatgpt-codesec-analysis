#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int longest_repeating_subsequence(const char *str, size_t *result)
{
    size_t length;
    size_t columns;
    size_t cells;
    size_t *table;

    if (str == NULL || result == NULL) {
        return -1;
    }

    length = strlen(str);

    if (length == SIZE_MAX) {
        return -1;
    }

    columns = length + 1;

    if (columns > SIZE_MAX / columns) {
        return -1;
    }

    cells = columns * columns;

    if (cells > SIZE_MAX / sizeof(*table)) {
        return -1;
    }

    table = calloc(cells, sizeof(*table));
    if (table == NULL) {
        return -1;
    }

    for (size_t i = 1; i <= length; ++i) {
        for (size_t j = 1; j <= length; ++j) {
            size_t current = i * columns + j;

            if (str[i - 1] == str[j - 1] && i != j) {
                table[current] = table[(i - 1) * columns + (j - 1)] + 1;
            } else {
                size_t above = table[(i - 1) * columns + j];
                size_t left = table[i * columns + (j - 1)];
                table[current] = above > left ? above : left;
            }
        }
    }

    *result = table[length * columns + length];
    free(table);
    return 0;
}

int main(void)
{
    char *input = NULL;
    size_t capacity = 0;
    size_t length = 0;
    size_t result;
    int character;
    int status = EXIT_FAILURE;

    while ((character = getchar()) != '\n' && character != EOF) {
        if (length == capacity) {
            size_t new_capacity;
            char *new_input;

            if (capacity == 0) {
                new_capacity = 64;
            } else {
                if (capacity > SIZE_MAX / 2) {
                    goto cleanup;
                }
                new_capacity = capacity * 2;
            }

            new_input = realloc(input, new_capacity);
            if (new_input == NULL) {
                goto cleanup;
            }

            input = new_input;
            capacity = new_capacity;
        }

        input[length++] = (char)character;
    }

    if (ferror(stdin)) {
        goto cleanup;
    }

    if (length == SIZE_MAX) {
        goto cleanup;
    }

    if (length + 1 > capacity) {
        char *new_input = realloc(input, length + 1);
        if (new_input == NULL) {
            goto cleanup;
        }
        input = new_input;
    }

    input[length] = '\0';

    if (longest_repeating_subsequence(input, &result) != 0) {
        goto cleanup;
    }

    if (printf("%zu\n", result) < 0) {
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    free(input);
    return status;
}