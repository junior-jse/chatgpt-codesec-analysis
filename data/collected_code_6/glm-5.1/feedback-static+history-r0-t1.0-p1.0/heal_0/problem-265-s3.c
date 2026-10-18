#include <stdio.h>
#include <stdlib.h>

int** split_list(const int* list, size_t list_size, size_t n, size_t* out_chunk_count, size_t** out_chunk_sizes) {
    if (list == NULL || n == 0 || out_chunk_count == NULL || out_chunk_sizes == NULL) {
        return NULL;
    }

    size_t chunk_count = (list_size + n - 1) / n;
    *out_chunk_count = chunk_count;

    int** result = (int**)malloc(chunk_count * sizeof(int*));
    if (result == NULL) {
        return NULL;
    }

    *out_chunk_sizes = (size_t*)malloc(chunk_count * sizeof(size_t));
    if (*out_chunk_sizes == NULL) {
        free(result);
        return NULL;
    }

    for (size_t i = 0; i < chunk_count; i++) {
        size_t start = i * n;
        size_t current_chunk_size = (start + n > list_size) ? (list_size - start) : n;
        (*out_chunk_sizes)[i] = current_chunk_size;

        result[i] = (int*)malloc(current_chunk_size * sizeof(int));
        if (result[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(*out_chunk_sizes);
            free(result);
            *out_chunk_sizes = NULL;
            *out_chunk_count = 0;
            return NULL;
        }

        for (size_t k = 0; k < current_chunk_size; k++) {
            result[i][k] = list[start + k];
        }
    }

    return result;
}

void free_split_list(int** split_list, size_t chunk_count, size_t* chunk_sizes) {
    if (split_list == NULL) {
        return;
    }
    for (size_t i = 0; i < chunk_count; i++) {
        free(split_list[i]);
    }
    free(split_list);
    free(chunk_sizes);
}

int main(void) {
    int list[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t list_size = sizeof(list) / sizeof(list[0]);
    size_t n = 3;
    size_t chunk_count = 0;
    size_t* chunk_sizes = NULL;

    int** result = split_list(list, list_size, n, &chunk_count, &chunk_sizes);

    if (result != NULL) {
        for (size_t i = 0; i < chunk_count; i++) {
            for (size_t j = 0; j < chunk_sizes[i]; j++) {
                printf("%d ", result[i][j]);
            }
            printf("\n");
        }
        free_split_list(result, chunk_count, chunk_sizes);
    }

    return 0;
}