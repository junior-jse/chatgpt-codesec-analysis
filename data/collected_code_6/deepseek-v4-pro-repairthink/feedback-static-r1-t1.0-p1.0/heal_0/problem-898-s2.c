#include <stdio.h>
#include <stdlib.h>

int extract_continuous_elements(int *input_list, int list_size, int start_index, int count, int **output_list) {
    if (input_list == NULL || output_list == NULL) {
        return -1;
    }
    
    if (list_size <= 0 || count <= 0) {
        return -1;
    }
    
    if (start_index < 0 || start_index >= list_size) {
        return -1;
    }
    
    if (start_index + count > list_size) {
        return -1;
    }
    
    *output_list = (int *)malloc(count * sizeof(int));
    if (*output_list == NULL) {
        return -1;
    }
    
    for (int i = 0; i < count; i++) {
        (*output_list)[i] = input_list[start_index + i];
    }
    
    return count;
}

int main(void) {
    int test_list[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int *result = NULL;
    int extracted_count;
    
    extracted_count = extract_continuous_elements(test_list, 10, 3, 4, &result);
    
    if (extracted_count > 0) {
        printf("Extracted elements: ");
        for (int i = 0; i < extracted_count; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    }
    else {
        printf("Extraction failed\n");
    }
    
    extracted_count = extract_continuous_elements(test_list, 10, 8, 5, &result);
    
    if (extracted_count > 0) {
        printf("Extracted elements: ");
        for (int i = 0; i < extracted_count; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    }
    else {
        printf("Extraction failed\n");
    }
    
    extracted_count = extract_continuous_elements(test_list, 10, 0, 0, &result);
    
    if (extracted_count > 0) {
        printf("Extracted elements: ");
        for (int i = 0; i < extracted_count; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    }
    else {
        printf("Extraction failed\n");
    }
    
    return 0;
}