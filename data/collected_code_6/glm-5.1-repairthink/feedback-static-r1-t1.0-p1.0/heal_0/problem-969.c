#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

Tuple* join_tuples(Tuple* input, size_t input_len, size_t* output_len) {
    if (input == NULL || output_len == NULL) {
        return NULL;
    }

    Tuple* result = (Tuple*)malloc(input_len * sizeof(Tuple));
    if (result == NULL) {
        return NULL;
    }

    size_t res_idx = 0;

    if (input_len > 0) {
        result[res_idx].first = input[0].first;
        result[res_idx].second = input[0].second;

        for (size_t i = 1; i < input_len; i++) {
            if (input[i].first == result[res_idx].first) {
                result[res_idx].second += input[i].second;
            } else {
                res_idx++;
                result[res_idx].first = input[i].first;
                result[res_idx].second = input[i].second;
            }
        }
        res_idx++;
    }

    *output_len = res_idx;

    Tuple* temp = (Tuple*)realloc(result, res_idx * sizeof(Tuple));
    if (temp != NULL || res_idx == 0) {
        result = temp;
    }

    return result;
}

int main(void) {
    Tuple data[] = {{1, 5}, {1, 10}, {2, 3}, {3, 7}, {3, 2}, {3, 1}, {4, 8}};
    size_t data_len = sizeof(data) / sizeof(data[0]);
    size_t out_len = 0;

    Tuple* joined = join_tuples(data, data_len, &out_len);

    if (joined == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < out_len; i++) {
        printf("(%d, %d)\n", joined[i].first, joined[i].second);
    }

    free(joined);
    joined = NULL;

    return EXIT_SUCCESS;
}