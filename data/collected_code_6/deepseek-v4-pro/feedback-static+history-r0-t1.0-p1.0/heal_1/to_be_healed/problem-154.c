#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Possible weaknesses found:
 *  Parameter 'indices' can be declared as pointer to const [constParameterPointer]
 */
int** extract_elements(int** input_array, int rows, int cols, int* indices, int indices_count, int** output_array, int** output_cols, int* output_rows) {
    if (input_array == NULL || indices == NULL || output_array == NULL || output_cols == NULL || output_rows == NULL) {
        return NULL;
    }
    
    if (rows <= 0 || cols <= 0 || indices_count <= 0) {
        return NULL;
    }
    
    for (int i = 0; i < indices_count; i++) {
        if (indices[i] < 0 || indices[i] >= cols) {
            return NULL;
        }
    }
    
    *output_array = (int*)malloc(rows * indices_count * sizeof(int));
    if (*output_array == NULL) {
        return NULL;
    }
    
    *output_cols = (int*)malloc(rows * sizeof(int));
    if (*output_cols == NULL) {
        free(*output_array);
        *output_array = NULL;
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        (*output_cols)[i] = indices_count;
        for (int j = 0; j < indices_count; j++) {
            (*output_array)[i * indices_count + j] = input_array[i][indices[j]];
        }
    }
    
    *output_rows = rows;
    /* Possible weaknesses found:
     *  returning 'int *' from a function with incompatible return type 'int **' [-Wincompatible-pointer-types]
     */
    return *output_array;
}

int** create_2d_array(int rows, int cols) {
    int** array = (int**)malloc(rows * sizeof(int*));
    if (array == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        array[i] = (int*)malloc(cols * sizeof(int));
        if (array[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(array[j]);
            }
            free(array);
            return NULL;
        }
    }
    
    return array;
}

void free_2d_array(int** array, int rows) {
    if (array == NULL) {
        return;
    }
    
    for (int i = 0; i < rows; i++) {
        free(array[i]);
    }
    free(array);
}

int main(void) {
    int rows = 3;
    int cols = 4;
    int** input = create_2d_array(rows, cols);
    if (input == NULL) {
        return 1;
    }
    
    int value = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            input[i][j] = value++;
        }
    }
    
    int indices[] = {0, 2};
    int indices_count = 2;
    int* output_array = NULL;
    int* output_cols = NULL;
    int output_rows = 0;
    
    if (extract_elements(input, rows, cols, indices, indices_count, &output_array, &output_cols, &output_rows) == NULL) {
        free_2d_array(input, rows);
        return 1;
    }
    
    for (int i = 0; i < output_rows; i++) {
        for (int j = 0; j < output_cols[i]; j++) {
            printf("%d ", output_array[i * indices_count + j]);
        }
        printf("\n");
    }
    
    free(output_array);
    free(output_cols);
    free_2d_array(input, rows);
    
    return 0;
}