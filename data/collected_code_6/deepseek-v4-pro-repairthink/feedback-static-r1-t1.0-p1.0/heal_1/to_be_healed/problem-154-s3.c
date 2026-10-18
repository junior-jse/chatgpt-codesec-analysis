#include <stdio.h>
#include <stdlib.h>

/* Possible weaknesses found:
 *  Parameter 'col_indices' can be declared as pointer to const [constParameterPointer]
 *  Parameter 'row_indices' can be declared as pointer to const [constParameterPointer]
 */
int* extract_elements(int** array, int rows, int cols, int* row_indices, int* col_indices, int count, int* result_size) {
    /* Possible weaknesses found:
     *  Assuming that condition 'result_size==NULL' is not redundant
     */
    if (array == NULL || row_indices == NULL || col_indices == NULL || result_size == NULL || count <= 0 || rows <= 0 || cols <= 0) {
        /* Possible weaknesses found:
         *  Either the condition 'result_size==NULL' is redundant or there is possible null pointer dereference: result_size. [nullPointerRedundantCheck]
         *  Null pointer dereference
         */
        *result_size = 0;
        return NULL;
    }
    
    int* result = (int*)malloc(count * sizeof(int));
    if (result == NULL) {
        *result_size = 0;
        return NULL;
    }
    
    int valid_count = 0;
    
    for (int i = 0; i < count; i++) {
        if (row_indices[i] >= 0 && row_indices[i] < rows && 
            col_indices[i] >= 0 && col_indices[i] < cols) {
            result[valid_count] = array[row_indices[i]][col_indices[i]];
            valid_count++;
        }
    }
    
    *result_size = valid_count;
    
    if (valid_count == 0) {
        free(result);
        return NULL;
    }
    
    if (valid_count < count) {
        int* temp = (int*)realloc(result, valid_count * sizeof(int));
        if (temp != NULL) {
            result = temp;
        }
    }
    
    return result;
}

int main() {
    int rows = 3;
    int cols = 4;
    
    int** array = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        array[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            array[i][j] = i * cols + j + 1;
        }
    }
    
    int row_indices[] = {0, 1, 2, 0, 5};
    int col_indices[] = {1, 2, 3, 0, 1};
    int count = 5;
    int result_size = 0;
    
    int* result = extract_elements(array, rows, cols, row_indices, col_indices, count, &result_size);
    
    if (result != NULL) {
        printf("Extracted elements: ");
        for (int i = 0; i < result_size; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    } else {
        printf("No valid elements extracted\n");
    }
    
    for (int i = 0; i < rows; i++) {
        free(array[i]);
    }
    free(array);
    
    return 0;
}