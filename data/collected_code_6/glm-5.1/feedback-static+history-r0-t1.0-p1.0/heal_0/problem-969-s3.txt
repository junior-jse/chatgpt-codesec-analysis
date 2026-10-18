#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
} Tuple;

typedef struct {
    int a;
    int b_sum;
} JoinedTuple;

JoinedTuple* join_tuples(Tuple* tuples, size_t n, size_t* out_size) {
    if (tuples == NULL || out_size == NULL || n == 0) {
        if (out_size != NULL) *out_size = 0;
        return NULL;
    }

    JoinedTuple* result = (JoinedTuple*)malloc(n * sizeof(JoinedTuple));
    if (result == NULL) {
        *out_size = 0;
        return NULL;
    }

    size_t r_idx = 0;
    result[r_idx].a = tuples[0].a;
    result[r_idx].b_sum = tuples[0].b;

    for (size_t i = 1; i < n; i++) {
        if (tuples[i].a == result[r_idx].a) {
            result[r_idx].b_sum += tuples[i].b;
        } else {
            r_idx++;
            result[r_idx].a = tuples[i].a;
            result[r_idx].b_sum = tuples[i].b;
        }
    }

    *out_size = r_idx + 1;

    JoinedTuple* shrunk = (JoinedTuple*)realloc(result, (r_idx + 1) * sizeof(JoinedTuple));
    if (shrunk != NULL) {
        result = shrunk;
    }

    return result;
}

int main(void) {
    Tuple data[] = {{1, 2}, {1, 5}, {2, 3}, {3, 1}, {3, 4}, {3, 6}};
    size_t n = sizeof(data) / sizeof(data[0]);

    size_t joined_n = 0;
    JoinedTuple* joined = join_tuples(data, n, &joined_n);

    if (joined != NULL) {
        for (size_t i = 0; i < joined_n; i++) {
            printf("(%d, %d)\n", joined[i].a, joined[i].b_sum);
        }
        free(joined);
        joined = NULL;
    }

    return 0;
}