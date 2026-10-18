#include <stdio.h>
#include <stdlib.h>

int **generate_2d_array(size_t rows, size_t cols, int init_value)
{
    int **array;
    size_t i;

    if (rows == 0 || cols == 0) {
        return NULL;
    }

    array = malloc(rows * sizeof(int *));
    if (array == NULL) {
        return NULL;
    }

    for (i = 0; i < rows; i++) {
        array[i] = malloc(cols * sizeof(int));
        if (array[i] == NULL) {
            size_t j;
            for (j = 0; j < i; j++) {
                free(array[j]);
            }
            free(array);
            return NULL;
        }

        size_t k;
        for (k = 0; k < cols; k++) {
            array[i][k] = init_value;
        }
    }

    return array;
}

void free_2d_array(int **array, size_t rows)
{
    size_t i;

    if (array == NULL) {
        return;
    }

    for (i = 0; i < rows; i++) {
        free(array[i]);
    }
    free(array);
}

int main(void)
{
    size_t rows = 5;
    size_t cols = 4;
    int **array;
    size_t i, j;

    array = generate_2d_array(rows, cols, 0);
    if (array == NULL) {
        fprintf(stderr, "Failed to allocate 2D array\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }

    free_2d_array(array, rows);

    return EXIT_SUCCESS;
}