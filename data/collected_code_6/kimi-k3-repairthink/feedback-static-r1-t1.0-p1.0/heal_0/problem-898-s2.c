#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* extract_continuous_elements(const int* list, size_t list_size, size_t start_index, size_t num_elements, size_t* extracted_size) {
    if (list == NULL || extracted_size == NULL) {
        return NULL;
    }
    
    if (start_index >= list_size) {
        *extracted_size = 0;
        return NULL;
    }
    
    size_t available = list_size - start_index;
    size_t elements_to_copy = (num_elements > available) ? available : num_elements;
    
    if (elements_to_copy == 0) {
        *extracted_size = 0;
        return NULL;
    }
    
    int* result = (int*)malloc(elements_to_copy * sizeof(int));
    if (result == NULL) {
        *extracted_size = 0;
        return NULL;
    }
    
    memcpy(result, list + start_index, elements_to_copy * sizeof(int));
    *extracted_size = elements_to_copy;
    
    return result;
}

int main(void) {
    int list[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t list_size = sizeof(list) / sizeof(list[0]);
    size_t extracted_size = 0;
    
    size_t start_index = 2;
    size_t num_elements = 4;
    
    int* extracted = extract_continuous_elements(list, list_size, start_index, num_elements, &extracted_size);
    
    if (extracted != NULL) {
        printf("Extracted %zu elements starting from index %zu:\n", extracted_size, start_index);
        for (size_t i = 0; i < extracted_size; i++) {
            printf("%d ", extracted[i]);
        }
        printf("\n");
        
        free(extracted);
    } else {
        printf("Failed to extract elements or invalid parameters.\n");
    }
    
    return 0;
}