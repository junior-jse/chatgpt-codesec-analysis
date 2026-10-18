#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct {
    Tuple tuple;
    int frequency;
} TupleFrequency;

static int compare_tuples(const void *a, const void *b)
{
    const Tuple *ta = (const Tuple *)a;
    const Tuple *tb = (const Tuple *)b;
    if (ta->first != tb->first) {
        return (ta->first > tb->first) - (ta->first < tb->first);
    }
    return (ta->second > tb->second) - (ta->second < tb->second);
}

static int assign_frequency(const Tuple *tuples, size_t count, TupleFrequency **result)
{
    Tuple *sorted = NULL;
    TupleFrequency *freq_array = NULL;
    size_t i;
    size_t unique_count = 0;

    if (tuples == NULL || result == NULL) {
        return -1;
    }

    if (count == 0) {
        *result = NULL;
        return 0;
    }

    sorted = (Tuple *)malloc(count * sizeof(Tuple));
    if (sorted == NULL) {
        return -1;
    }
    memcpy(sorted, tuples, count * sizeof(Tuple));

    qsort(sorted, count, sizeof(Tuple), compare_tuples);

    for (i = 0; i < count; i++) {
        if (i == 0 || compare_tuples(&sorted[i - 1], &sorted[i]) != 0) {
            unique_count++;
        }
    }

    freq_array = (TupleFrequency *)malloc(unique_count * sizeof(TupleFrequency));
    if (freq_array == NULL) {
        free(sorted);
        return -1;
    }

    unique_count = 0;
    for (i = 0; i < count; i++) {
        if (i == 0 || compare_tuples(&sorted[i - 1], &sorted[i]) != 0) {
            freq_array[unique_count].tuple = sorted[i];
            freq_array[unique_count].frequency = 1;
            unique_count++;
        } else {
            freq_array[unique_count - 1].frequency++;
        }
    }

    free(sorted);
    *result = freq_array;
    return (int)unique_count;
}

int main(void)
{
    Tuple tuples[] = {
        {1, 2}, {3, 4}, {1, 2}, {5, 6}, {3, 4}, {1, 2}
    };
    size_t count = sizeof(tuples) / sizeof(tuples[0]);
    TupleFrequency *result = NULL;
    int unique_count;
    int i;

    unique_count = assign_frequency(tuples, count, &result);
    if (unique_count < 0) {
        fprintf(stderr, "Error: Failed to assign frequencies\n");
        return EXIT_FAILURE;
    }

    printf("Tuple frequencies:\n");
    for (i = 0; i < unique_count; i++) {
        printf("(%d, %d): %d\n", result[i].tuple.first, result[i].tuple.second, result[i].frequency);
    }

    free(result);
    return EXIT_SUCCESS;
}