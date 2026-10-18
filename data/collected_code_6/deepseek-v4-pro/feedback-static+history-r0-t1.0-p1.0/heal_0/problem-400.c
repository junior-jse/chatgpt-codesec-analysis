#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int key1;
    int key2;
    int freq;
} TupleFreq;

static int compare_tuples(const void *a, const void *b) {
    const int *tuple_a = (const int *)a;
    const int *tuple_b = (const int *)b;
    
    if (tuple_a[0] != tuple_b[0]) {
        return tuple_a[0] - tuple_b[0];
    }
    
    return tuple_a[1] - tuple_b[1];
}

static int tuple_exists(TupleFreq *freqs, int count, int v1, int v2) {
    for (int i = 0; i < count; i++) {
        if ((freqs[i].key1 == v1 && freqs[i].key2 == v2) ||
            (freqs[i].key1 == v2 && freqs[i].key2 == v1)) {
            return i;
        }
    }
    return -1;
}

int extract_tuple_frequency(const int *list, int list_size, int tuple_size,
                            TupleFreq **result, int *result_size) {
    if (list == NULL || result == NULL || result_size == NULL) {
        return -1;
    }
    
    if (list_size < tuple_size || tuple_size < 2) {
        return -1;
    }
    
    int max_tuples = list_size * (list_size - 1) / 2;
    TupleFreq *freqs = (TupleFreq *)malloc(max_tuples * sizeof(TupleFreq));
    if (freqs == NULL) {
        return -1;
    }
    
    int count = 0;
    
    for (int i = 0; i < list_size; i += tuple_size) {
        if (i + tuple_size <= list_size) {
            for (int j = 0; j < tuple_size; j++) {
                for (int k = j + 1; k < tuple_size; k++) {
                    int v1 = list[i + j];
                    int v2 = list[i + k];
                    
                    int idx = tuple_exists(freqs, count, v1, v2);
                    if (idx >= 0) {
                        freqs[idx].freq++;
                    } else {
                        freqs[count].key1 = v1;
                        freqs[count].key2 = v2;
                        freqs[count].freq = 1;
                        count++;
                    }
                }
            }
        }
    }
    
    if (count == 0) {
        free(freqs);
        *result = NULL;
        *result_size = 0;
        return 0;
    }
    
    TupleFreq *resized = (TupleFreq *)realloc(freqs, count * sizeof(TupleFreq));
    if (resized == NULL) {
        free(freqs);
        return -1;
    }
    
    *result = resized;
    *result_size = count;
    
    return 0;
}

int main(void) {
    int list[] = {1, 2, 3, 1, 2, 4, 2, 3, 1, 3, 4, 1, 2, 5, 3, 4, 1, 2, 2, 3};
    int list_size = sizeof(list) / sizeof(list[0]);
    int tuple_size = 3;
    TupleFreq *result = NULL;
    int result_size = 0;
    
    int status = extract_tuple_frequency(list, list_size, tuple_size, &result, &result_size);
    
    if (status == 0) {
        printf("Tuple frequencies (order independent):\n");
        for (int i = 0; i < result_size; i++) {
            printf("(%d, %d): %d\n", result[i].key1, result[i].key2, result[i].freq);
        }
        free(result);
    } else {
        printf("Error extracting tuple frequencies\n");
    }
    
    return 0;
}