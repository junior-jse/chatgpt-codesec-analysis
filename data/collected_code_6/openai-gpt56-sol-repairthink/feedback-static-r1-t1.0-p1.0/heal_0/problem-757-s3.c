#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int are_reverses(const char *first, const char *second)
{
    size_t first_len;
    size_t second_len;
    size_t i;

    if (first == NULL || second == NULL) {
        return 0;
    }

    first_len = strlen(first);
    second_len = strlen(second);

    if (first_len != second_len) {
        return 0;
    }

    for (i = 0; i < first_len; ++i) {
        if (first[i] != second[first_len - i - 1U]) {
            return 0;
        }
    }

    return 1;
}

static int count_reverse_pairs(char *const strings[], size_t count, size_t *result)
{
    size_t pairs = 0;
    size_t i;
    size_t j;

    if (result == NULL || (count != 0U && strings == NULL)) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        if (strings[i] == NULL) {
            return -1;
        }

        for (j = i + 1U; j < count; ++j) {
            if (strings[j] == NULL) {
                return -1;
            }

            if (are_reverses(strings[i], strings[j])) {
                if (pairs == SIZE_MAX) {
                    return -1;
                }
                ++pairs;
            }
        }
    }

    *result = pairs;
    return 0;
}

int main(void)
{
    char *strings[] = {
        "abc",
        "cba",
        "hello",
        "olleh",
        "abc",
        "level"
    };
    size_t count = sizeof(strings) / sizeof(strings[0]);
    size_t pairs;

    if (count_reverse_pairs(strings, count, &pairs) != 0) {
        fputs("Failed to count reverse-string pairs.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%zu\n", pairs) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}