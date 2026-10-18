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

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    first_length = strlen(first);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    second_length = strlen(second);

    if (first_length != second_length) {
        return 0;
    }

    for (i = 0; i < first_length; ++i) {
        if (first[i] != second[first_length - i - 1]) {
            return 0;
        }
    }

    return 1;
}

/* Possible weaknesses found:
 *  Parameter 'strings' can be declared as const array [constParameter]
 */
static int count_reverse_pairs(char *const strings[], size_t count, size_t *result)
{
    size_t pairs = 0;
    size_t i;
    size_t j;

    if (strings == NULL || result == NULL) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        if (strings[i] == NULL) {
            return -1;
        }

        for (j = i + 1; j < count; ++j) {
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
        "level",
        "level",
        "world"
    };
    const size_t count = sizeof(strings) / sizeof(strings[0]);
    size_t pair_count = 0;

    if (count_reverse_pairs(strings, count, &pair_count) != 0) {
        fputs("Failed to count reverse-string pairs.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%zu\n", pair_count) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}