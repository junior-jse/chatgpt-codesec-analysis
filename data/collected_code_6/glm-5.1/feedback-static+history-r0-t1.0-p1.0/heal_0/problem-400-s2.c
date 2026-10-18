#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int a;
    int b;
} Tuple;

typedef struct {
    Tuple t;
    int freq;
} TupleFreq;

int match_tuple(const void *a, const void *b) {
    const Tuple *ta = (const Tuple *)a;
    const Tuple *tb = (const Tuple *)b;
    return (ta->a == tb->a && ta->b == tb->b) ? 0 : 1;
}

int extract_unique_freqs(Tuple *arr, int n, TupleFreq **out) {
    if (n == 0 || arr == NULL || out == NULL) {
        if (out) *out = NULL;
        return 0;
    }

    TupleFreq *res = (TupleFreq *)malloc(n * sizeof(TupleFreq));
    if (res == NULL) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < count; j++) {
            if (match_tuple(&arr[i], &res[j].t) == 0) {
                res[j].freq++;
                found = 1;
                break;
            }
        }
        if (found == 0) {
            res[count].t = arr[i];
            res[count].freq = 1;
            count++;
        }
    }

    TupleFreq *trimmed = (TupleFreq *)realloc(res, count * sizeof(TupleFreq));
    if (trimmed != NULL) {
        res = trimmed;
    }
    
    *out = res;
    return count;
}

int main() {
    Tuple data[] = {{1, 2}, {3, 4}, {1, 2}, {5, 6}, {3, 4}, {1, 2}};
    int n = sizeof(data) / sizeof(data[0]);
    
    TupleFreq *freqs = NULL;
    int unique_count = extract_unique_freqs(data, n, &freqs);
    
    if (unique_count > 0 && freqs != NULL) {
        for (int i = 0; i < unique_count; i++) {
            printf("(%d, %d): %d\n", freqs[i].t.a, freqs[i].t.b, freqs[i].freq);
        }
        free(freqs);
    } else {
        printf("No data or allocation failed.\n");
    }
    
    return 0;
}