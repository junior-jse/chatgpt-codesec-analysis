#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
    size_t index = 0;
    Tuple result;

    if (printf("Enter a zero-based tuple index: ") < 0) {
        return EXIT_FAILURE;
    }

    if (scanf("%zu", &index) != 1) {
        fputs("Invalid input.\n", stderr);
        return EXIT_FAILURE;
    }

    if (!extract_nth(tuples, count, index, &result)) {
        fputs("Index out of range.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("(%d, %d)\n", result.first, result.second) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}