#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int length;
} SublistResult;

SublistResult* find_minimum_length_sublist(const int* array, int size, int target) {
    if (array == NULL || size <= 0) {
        return NULL;
    }
    
    SublistResult* result = (SublistResult*)malloc(sizeof(SublistResult));
    if (result == NULL) {
        return NULL;
    }
    
    result->start = -1;
    result->length = 0;
    int min_length = size + 1;
    int current_sum = 0;
    int window_start = 0;
    
    for (int window_end = 0; window_end < size; window_end++) {
        current_sum += array[window_end];
        
        while (current_sum >= target && window_start <= window_end) {
            int current_length = window_end - window_start + 1;
            if (current_length < min_length) {
                min_length = current_length;
                result->start = window_start;
                result->length = current_length;
            }
            current_sum -= array[window_start];
            window_start++;
        }
    }
    
    if (result->start == -1) {
        free(result);
        return NULL;
    }
    
    return result;
}

int main(void) {
    int array[] = {2, 3, 1, 2, 4, 3};
    int size = sizeof(array) / sizeof(array[0]);
    int target = 7;
    
    SublistResult* result = find_minimum_length_sublist(array, size, target);
    
    if (result != NULL) {
        printf("Start index: %d\n", result->start);
        printf("Length: %d\n", result->length);
        printf("Sublist: ");
        for (int i = result->start; i < result->start + result->length; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
        free(result);
    } else {
        printf("No sublist found with sum >= %d\n", target);
    }
    
    return 0;
}