#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static int compare_integers(const void *left, const void *right)
{
    const int a = *(const int *)left;
    const int b = *(const int *)right;

    return (a > b) - (a < b);
}

static bool find_most_frequent(const int *items, size_t count, int *result)
{
    if (items == NULL || result == NULL || count == 0 ||
        count > SIZE_MAX / sizeof(*items)) {
        return false;
    }

    int *sorted = malloc(count * sizeof(*sorted));
    if (sorted == NULL) {
        return false;
    }

    for (size_t i = 0; i < count; ++i) {
        sorted[i] = items[i];
    }

    qsort(sorted, count, sizeof(*sorted), compare_integers);

    int most_frequent = sorted[0];
    size_t maximum_frequency = 1;
    size_t current_frequency = 1;

    for (size_t i = 1; i < count; ++i) {
        if (sorted[i] == sorted[i - 1]) {
            ++current_frequency;
        } else {
            if (current_frequency > maximum_frequency) {
                maximum_frequency = current_frequency;
                most_frequent = sorted[i - 1];
            }
            current_frequency = 1;
        }
    }

    if (current_frequency > maximum_frequency) {
        most_frequent = sorted[count - 1];
    }

    *result = most_frequent;
    free(sorted);
    return true;
}

int main(void)
{
    int items[] = {4, 2, 7, 4, 3, 2, 4, 7, 4};
    const size_t count = sizeof(items) / sizeof(items[0]);
    int most_frequent = 0;

    if (!find_most_frequent(items, count, &most_frequent)) {
        fputs("Unable to find the most frequent item.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%d\n", most_frequent) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}