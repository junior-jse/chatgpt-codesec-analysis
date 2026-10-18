#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int a;
    int b;
} Tuple;

Tuple* join_tuples(Tuple* tuples, size_t n, size_t* out_size) {
    if (tuples == NULL || n == 0 || out_size == NULL) {
        if (out_size != NULL) {
            *out_size = 0;
        }
        return NULL;
    }

    Tuple* result = (Tuple*)malloc(n * sizeof(Tuple));
    if (result == NULL) {
        *out_size = 0;
        return NULL;
    }

    result[0] = tuples[0];
    size_t count = 1;

    for (size_t i = 1; i < n; i++) {
        if (tuples[i].a == result[count - 1].a) {
            result[count - 1].b += tuples[i].b;
        } else {
            result[count] = tuples[i];
            count++;
        }
    }

    Tuple* resized = (Tuple*)realloc(result, count * sizeof(Tuple));
    if (resized != NULL) {
        result = resized;
    }

    *out_size = count;
    return result;
}

int main() {
    Tuple tuples[] = {{1, 5}, {1, 10}, {2, 3}, {3, 7}, {3, 2}, {4, 1}};
    size_t n = sizeof(tuples) / sizeof(tuples[0]);
    size_t out_size = 0;

    Tuple* joined = join_tuples(tuples, n, &out_size);

    if (joined != NULL) {
        for (size_t i = 0; i < out_size; i++) {
            printf("(%d, %d)\n", joined[i].a, joined[i].b);
        }
        free(joined);
    }

    return 0;
}