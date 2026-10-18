#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int value;
    size_t count;
    size_t first_index;
} RecordFrequency;

static int compare_integers(const void *left, const void *right)
{
    const int a = *(const int *)left;
    const int b = *(const int *)right;

    return (a > b) - (a < b);
}

static int find_similar_occurrences(const int *records, size_t count)
{
    int *sorted = NULL;
    RecordFrequency *frequencies = NULL;
    size_t unique_count = 0;
    size_t i;
    int found = 0;

    if (records == NULL || count == 0 || count > SIZE_MAX / sizeof(*sorted)) {
        return -1;
    }

    sorted = malloc(count * sizeof(*sorted));
    if (sorted == NULL) {
        return -1;
    }

    frequencies = malloc(count * sizeof(*frequencies));
    if (frequencies == NULL) {
        free(sorted);
        return -1;
    }

    for (i = 0; i < count; ++i) {
        sorted[i] = records[i];
    }

    qsort(sorted, count, sizeof(*sorted), compare_integers);

    for (i = 0; i < count;) {
        size_t next = i + 1;

        while (next < count && sorted[next] == sorted[i]) {
            ++next;
        }

        frequencies[unique_count].value = sorted[i];
        frequencies[unique_count].count = next - i;
        frequencies[unique_count].first_index = i;
        ++unique_count;
        i = next;
    }

    for (i = 0; i < unique_count; ++i) {
        size_t j;

        for (j = i + 1; j < unique_count; ++j) {
            if (frequencies[i].count == frequencies[j].count) {
                if (printf("%d and %d each occur %zu times\n",
                           frequencies[i].value,
                           frequencies[j].value,
                           frequencies[i].count) < 0) {
                    free(frequencies);
                    free(sorted);
                    return -1;
                }
                found = 1;
            }
        }
    }

    if (!found && printf("No records occur the same number of times\n") < 0) {
        free(frequencies);
        free(sorted);
        return -1;
    }

    free(frequencies);
    free(sorted);
    return found;
}

int main(void)
{
    const int tuples[] = {
        10, 20, 10, 30, 20, 40, 30, 10, 40, 20, 50
    };
    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    const int result = find_similar_occurrences(tuples, tuple_count);

    if (result < 0) {
        if (fprintf(stderr, "Unable to process records\n") < 0) {
            return EXIT_FAILURE;
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}