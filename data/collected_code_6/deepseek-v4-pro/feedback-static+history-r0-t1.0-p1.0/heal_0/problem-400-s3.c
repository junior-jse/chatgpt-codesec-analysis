#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct {
    Tuple *tuple;
    int frequency;
} TupleFrequency;

Tuple *create_tuple(int first, int second) {
    Tuple *tuple = (Tuple *)malloc(sizeof(Tuple));
    if (tuple == NULL) {
        return NULL;
    }
    tuple->first = first;
    tuple->second = second;
    return tuple;
}

int tuples_equal(const Tuple *a, const Tuple *b) {
    if (a == NULL || b == NULL) {
        return 0;
    }
    return (a->first == b->first && a->second == b->second) ||
           (a->first == b->second && a->second == b->first);
}

TupleFrequency *find_frequency(TupleFrequency *freq_array, int size, const Tuple *tuple) {
    if (freq_array == NULL || tuple == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        if (freq_array[i].tuple != NULL && tuples_equal(freq_array[i].tuple, tuple)) {
            return &freq_array[i];
        }
    }
    return NULL;
}

TupleFrequency *extract_tuple_frequencies(const Tuple *tuples, int tuple_count, int *result_count) {
    if (tuples == NULL || result_count == NULL || tuple_count <= 0) {
        if (result_count != NULL) {
            *result_count = 0;
        }
        return NULL;
    }

    TupleFrequency *freq_array = (TupleFrequency *)calloc(tuple_count, sizeof(TupleFrequency));
    if (freq_array == NULL) {
        *result_count = 0;
        return NULL;
    }

    int unique_count = 0;

    for (int i = 0; i < tuple_count; i++) {
        Tuple *current_tuple = create_tuple(tuples[i].first, tuples[i].second);
        if (current_tuple == NULL) {
            for (int j = 0; j < unique_count; j++) {
                if (freq_array[j].tuple != NULL) {
                    free(freq_array[j].tuple);
                }
            }
            free(freq_array);
            *result_count = 0;
            return NULL;
        }

        TupleFrequency *existing = find_frequency(freq_array, unique_count, current_tuple);
        if (existing != NULL) {
            existing->frequency++;
            free(current_tuple);
        } else {
            freq_array[unique_count].tuple = current_tuple;
            freq_array[unique_count].frequency = 1;
            unique_count++;
        }
    }

    TupleFrequency *result = (TupleFrequency *)malloc(unique_count * sizeof(TupleFrequency));
    if (result == NULL) {
        for (int i = 0; i < unique_count; i++) {
            if (freq_array[i].tuple != NULL) {
                free(freq_array[i].tuple);
            }
        }
        free(freq_array);
        *result_count = 0;
        return NULL;
    }

    for (int i = 0; i < unique_count; i++) {
        result[i] = freq_array[i];
    }

    free(freq_array);
    *result_count = unique_count;
    return result;
}

void free_tuple_frequencies(TupleFrequency *freq_array, int count) {
    if (freq_array == NULL) {
        return;
    }
    for (int i = 0; i < count; i++) {
        if (freq_array[i].tuple != NULL) {
            free(freq_array[i].tuple);
        }
    }
    free(freq_array);
}

int main(void) {
    Tuple tuples[] = {
        {1, 2},
        {3, 4},
        {2, 1},
        {5, 6},
        {4, 3},
        {1, 2},
        {7, 8}
    };
    int tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    int result_count = 0;

    TupleFrequency *frequencies = extract_tuple_frequencies(tuples, tuple_count, &result_count);

    if (frequencies == NULL) {
        fprintf(stderr, "Error: Failed to extract tuple frequencies\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < result_count; i++) {
        printf("Tuple (%d, %d): %d\n", 
               frequencies[i].tuple->first, 
               frequencies[i].tuple->second, 
               frequencies[i].frequency);
    }

    free_tuple_frequencies(frequencies, result_count);
    return EXIT_SUCCESS;
}