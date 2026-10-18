#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int val1;
    int val2;
    int val3;
} Tuple;

Tuple* remove_all_none_tuples(Tuple* list, size_t count, size_t* out_count) {
    if (list == NULL || out_count == NULL) {
        if (out_count != NULL) {
            *out_count = 0;
        }
        return NULL;
    }

    Tuple* result = (Tuple*)malloc(count * sizeof(Tuple));
    if (result == NULL) {
        *out_count = 0;
        return NULL;
    }

    size_t valid_count = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i].val1 != 0 || list[i].val2 != 0 || list[i].val3 != 0) {
            result[valid_count++] = list[i];
        }
    }

    if (valid_count == 0) {
        free(result);
        *out_count = 0;
        return NULL;
    }

    Tuple* shrunk_result = (Tuple*)realloc(result, valid_count * sizeof(Tuple));
    if (shrunk_result != NULL) {
        result = shrunk_result;
    }

    *out_count = valid_count;
    return result;
}

int main() {
    Tuple list[] = {
        {0, 0, 0},
        {1, 2, 3},
        {0, 0, 0},
        {4, 0, 5},
        {0, 0, 0}
    };
    size_t count = sizeof(list) / sizeof(list[0]);
    size_t out_count = 0;

    Tuple* filtered = remove_all_none_tuples(list, count, &out_count);

    for (size_t i = 0; i < out_count; i++) {
        printf("(%d, %d, %d)\n", filtered[i].val1, filtered[i].val2, filtered[i].val3);
    }

    free(filtered);

    return 0;
}