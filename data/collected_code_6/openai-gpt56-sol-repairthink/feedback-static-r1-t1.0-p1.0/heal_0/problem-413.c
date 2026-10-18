#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int first;
    int second;
} Tuple;

static int extract_nth(const Tuple *tuples, size_t count, size_t index, Tuple *result)
{
    if (tuples == NULL || result == NULL || index >= count) {
        return 0;
    }

    *result = tuples[index];
    return 1;
}

int main(void)
{
    const Tuple tuples[] = {
        {1, 10},
        {2, 20},
        {3, 30},
        {4, 40}
    };
    const size_t count = sizeof tuples / sizeof tuples[0];
    const size_t index = 2U;
    Tuple result;

    if (!extract_nth(tuples, count, index, &result)) {
        fputs("Unable to extract tuple.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("(%d, %d)\n", result.first, result.second) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}