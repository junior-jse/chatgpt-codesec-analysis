#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct {
    Tuple tuple;
    int count;
} TupleCount;

typedef struct {
    int frequency;
    int num_records;
} FreqGroup;

int match_tuple(Tuple a, Tuple b) {
    return a.first == b.first && a.second == b.second;
}

/* Possible weaknesses found:
 *  Parameter 'tuples' can be declared as pointer to const [constParameterPointer]
 */
void check_similar_occurrences(Tuple *tuples, int size) {
    if (size <= 0) {
        return;
    }

    TupleCount *counts = (TupleCount *)malloc(size * sizeof(TupleCount));
    if (!counts) {
        return;
    }

    int unique_size = 0;

    for (int i = 0; i < size; ++i) {
        int found = 0;
        for (int j = 0; j < unique_size; ++j) {
            if (match_tuple(tuples[i], counts[j].tuple)) {
                counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            counts[unique_size].tuple = tuples[i];
            counts[unique_size].count = 1;
            unique_size++;
        }
    }

    FreqGroup *groups = (FreqGroup *)malloc(unique_size * sizeof(FreqGroup));
    if (!groups) {
        free(counts);
        return;
    }

    int group_size = 0;

    for (int i = 0; i < unique_size; ++i) {
        int found = 0;
        for (int j = 0; j < group_size; ++j) {
            if (counts[i].count == groups[j].frequency) {
                groups[j].num_records++;
                found = 1;
                break;
            }
        }
        if (!found) {
            groups[group_size].frequency = counts[i].count;
            groups[group_size].num_records = 1;
            group_size++;
        }
    }

    for (int i = 0; i < group_size; ++i) {
        if (groups[i].num_records > 1) {
            printf("Frequency %d occurs for %d records\n", groups[i].frequency, groups[i].num_records);
        }
    }

    free(groups);
    free(counts);
}

int main() {
    Tuple data[] = {
        {1, 2},
        {3, 4},
        {1, 2},
        {5, 6},
        {3, 4},
        {7, 8}
    };

    int size = sizeof(data) / sizeof(data[0]);

    check_similar_occurrences(data, size);

    return 0;
}