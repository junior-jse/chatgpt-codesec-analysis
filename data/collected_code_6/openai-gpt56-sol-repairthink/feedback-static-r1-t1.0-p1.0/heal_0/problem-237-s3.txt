#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct {
    Tuple tuple;
    size_t count;
} Record;

static int compare_tuples(const void *left, const void *right)
{
    const Tuple *a = left;
    const Tuple *b = right;

    if (a->first < b->first) {
        return -1;
    }
    if (a->first > b->first) {
        return 1;
    }
    if (a->second < b->second) {
        return -1;
    }
    if (a->second > b->second) {
        return 1;
    }

    return 0;
}

static int check_similar_occurrences(Tuple *tuples, size_t tuple_count)
{
    Record *records = NULL;
    size_t record_count = 0;

    if (tuples == NULL && tuple_count != 0) {
        return -1;
    }

    if (tuple_count == 0) {
        return 0;
    }

    if (tuple_count > SIZE_MAX / sizeof(*records)) {
        return -1;
    }

    records = malloc(tuple_count * sizeof(*records));
    if (records == NULL) {
        return -1;
    }

    qsort(tuples, tuple_count, sizeof(*tuples), compare_tuples);

    for (size_t i = 0; i < tuple_count;) {
        size_t j = i + 1;

        while (j < tuple_count &&
               tuples[j].first == tuples[i].first &&
               tuples[j].second == tuples[i].second) {
            ++j;
        }

        records[record_count].tuple = tuples[i];
        records[record_count].count = j - i;
        ++record_count;
        i = j;
    }

    for (size_t i = 0; i < record_count; ++i) {
        for (size_t j = i + 1; j < record_count; ++j) {
            if (records[i].count == records[j].count) {
                if (printf("(%d, %d) and (%d, %d) occur %zu times\n",
                           records[i].tuple.first,
                           records[i].tuple.second,
                           records[j].tuple.first,
                           records[j].tuple.second,
                           records[i].count) < 0) {
                    free(records);
                    return -1;
                }
            }
        }
    }

    free(records);
    return 0;
}

int main(void)
{
    Tuple tuples[] = {
        {1, 2}, {3, 4}, {1, 2}, {5, 6},
        {3, 4}, {7, 8}, {5, 6}, {9, 10}
    };
    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);

    if (check_similar_occurrences(tuples, tuple_count) != 0) {
        fputs("Failed to check tuple occurrences\n", stderr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}