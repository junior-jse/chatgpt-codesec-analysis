#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int are_reverses(const char *first, const char *second)
{
    size_t first_length;
    size_t second_length;
    size_t i;

    if (first == NULL || second == NULL) {
        return 0;
    }

    first_length = strlen(first);
    second_length = strlen(second);

    if (first_length != second_length) {
        return 0;
    }

    for (i = 0; i < first_length; ++i) {
        if (first[i] != second[first_length - i - 1U]) {
            return 0;
        }
    }

    return 1;
}

static size_t count_reverse_pairs(char *const strings[], size_t count)
{
    size_t pairs = 0;
    size_t i;
    size_t j;

    if (strings == NULL) {
        return 0;
    }

    for (i = 0; i < count; ++i) {
        for (j = i + 1U; j < count; ++j) {
            if (are_reverses(strings[i], strings[j])) {
                ++pairs;
            }
        }
    }

    return pairs;
}

int main(void)
{
    char **strings = NULL;
    size_t count;
    size_t i;
    size_t capacity = 0;
    char *line = NULL;
    ssize_t length;
    size_t pairs;
    int status = EXIT_FAILURE;

    if (scanf("%zu", &count) != 1) {
        return EXIT_FAILURE;
    }

    if (getchar() == EOF && count != 0U) {
        return EXIT_FAILURE;
    }

    if (count != 0U) {
        if (count > SIZE_MAX / sizeof(*strings)) {
            return EXIT_FAILURE;
        }

        strings = calloc(count, sizeof(*strings));
        if (strings == NULL) {
            return EXIT_FAILURE;
        }
    }

    for (i = 0; i < count; ++i) {
        length = getline(&line, &capacity, stdin);
        if (length < 0) {
            goto cleanup;
        }

        if (length > 0 && line[(size_t)length - 1U] == '\n') {
            line[--length] = '\0';
        }

        if (length > 0 && line[(size_t)length - 1U] == '\r') {
            line[--length] = '\0';
        }

        strings[i] = malloc((size_t)length + 1U);
        if (strings[i] == NULL) {
            goto cleanup;
        }

        memcpy(strings[i], line, (size_t)length + 1U);
    }

    pairs = count_reverse_pairs(strings, count);

    if (printf("%zu\n", pairs) < 0) {
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    free(line);

    for (i = 0; i < count; ++i) {
        free(strings[i]);
    }

    free(strings);
    return status;
}