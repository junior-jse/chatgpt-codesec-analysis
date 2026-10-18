#include <stdio.h>
#include <stdlib.h>

int **split_list(const int *list, size_t length, size_t n, size_t *out_sub_count, size_t **out_sub_lengths) {
    if (list == NULL || out_sub_count == NULL || out_sub_lengths == NULL) {
        return NULL;
    }

    if (length == 0 || n == 0) {
        *out_sub_count = 0;
        *out_sub_lengths = NULL;
        return NULL;
    }

    size_t sub_count = (length + n - 1) / n;
    size_t *sub_lengths = malloc(sub_count * sizeof(size_t));
    if (sub_lengths == NULL) {
        return NULL;
    }

    int **result = malloc(sub_count * sizeof(int *));
    if (result == NULL) {
        free(sub_lengths);
        return NULL;
    }

    size_t i;
    for (i = 0; i < sub_count; i++) {
        sub_lengths[i] = (i == sub_count - 1) ? (length - i * n) : n;
        result[i] = malloc(sub_lengths[i] * sizeof(int));
        if (result[i] == NULL) {
            size_t j;
            for (j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            free(sub_lengths);
            return NULL;
        }
        size_t k;
        for (k = 0; k < sub_lengths[i]; k++) {
            result[i][k] = list[i * n + k];
        }
    }

    *out_sub_count = sub_count;
    *out_sub_lengths = sub_lengths;
    return result;
}

void free_split_list(int **split_list, size_t sub_count, size_t *sub_lengths) {
    if (split_list != NULL) {
        size_t i;
        for (i = 0; i < sub_count; i++) {
            free(split_list[i]);
        }
        free(split_list);
    }
    free(sub_lengths);
}

int main(void) {
    int list[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t length = sizeof(list) / sizeof(list[0]);
    size_t n = 3;
    size_t sub_count;
    size_t *sub_lengths;

    int **result = split_list(list, length, n, &sub_count, &sub_lengths);
    if (result != NULL) {
        size_t i;
        for (i = 0; i < sub_count; i++) {
            size_t j;
            for (j = 0; j < sub_lengths[i]; j++) {
                printf("%d ", result[i][j]);
            }
            printf("\n");
        }
        free_split_list(result, sub_count, sub_lengths);
    }

    return 0;
}