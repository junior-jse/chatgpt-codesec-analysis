#include <stdio.h>
#include <stdlib.h>

int **generate_2d_array(size_t rows, size_t cols, int initial_value);
void free_2d_array(int **array, size_t rows);

int **generate_2d_array(size_t rows, size_t cols, int initial_value)
{
    int **array;
    size_t i;
    size_t j;

    if (rows == 0 || cols == 0) {
        return NULL;
    }

    array = malloc(rows * sizeof(*array));
    if (array == NULL) {
        return NULL;
    }

    for (i = 0; i < rows; i++) {
        array[i] = malloc(cols * sizeof(**array));
        if (array[i] == NULL) {
            while (i > 0) {
                i--;
                free(array[i]);
            }
            free(array);
            return NULL;
        }
        for (j = 0; j < cols; j++) {
            array[i][j] = initial_value;
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
    size_t rows = 3;
    size_t cols = 4;
    int **array;
    size_t i;
    size_t j;

    array = generate_2d_array(rows, cols, 0);
    if (array == NULL) {
        fprintf(stderr, "Failed to allocate array\n");
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