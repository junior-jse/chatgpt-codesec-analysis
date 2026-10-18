#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int compare_ints(const void *lhs, const void *rhs)
{
    const int left = *(const int *)lhs;
    const int right = *(const int *)rhs;

    return (left > right) - (left < right);
}

static int find_most_frequent(const int *items, size_t count, int *result)
{
    if (items == NULL || result == NULL || count == 0U ||
        count > SIZE_MAX / sizeof(*items)) {
        return -1;
    }

    int *sorted = malloc(count * sizeof(*sorted));
    if (sorted == NULL) {
        return -1;
    }

    for (size_t i = 0U; i < count; ++i) {
        sorted[i] = items[i];
    }

    qsort(sorted, count, sizeof(*sorted), compare_ints);

    int most_frequent = sorted[0];
    size_t maximum_frequency = 1U;
    size_t current_frequency = 1U;

    for (size_t i = 1U; i < count; ++i) {
        if (sorted[i] == sorted[i - 1U]) {
            ++current_frequency;
        } else {
            if (current_frequency > maximum_frequency) {
                maximum_frequency = current_frequency;
                most_frequent = sorted[i - 1U];
            }
            current_frequency = 1U;
        }
    }

    if (current_frequency > maximum_frequency) {
        most_frequent = sorted[count - 1U];
    }

    *result = most_frequent;
    free(sorted);
    return 0;
}

int main(void)
{
    int items[] = {4, 2, 7, 4, 3, 4, 2, 7, 7, 7};
    const size_t count = sizeof(items) / sizeof(items[0]);
    int result = 0;

    if (find_most_frequent(items, count, &result) != 0) {
        fputs("Unable to determine the most frequent item.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%d\n", result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}