#include <stdio.h>
#include <stdlib.h>

int count_elements_in_range(const int arr[], int size, int lower, int upper) {
    if (arr == NULL || size <= 0 || lower > upper) {
        return -1;
    }
    
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] >= lower && arr[i] <= upper) {
            count++;
        }
    }
    return count;
}

int main(void) {
    int size;
    
    printf("Enter the number of elements: ");
    if (scanf("%d", &size) != 1 || size <= 0) {
        fprintf(stderr, "Invalid input for size\n");
        return EXIT_FAILURE;
    }
    
    int *arr = malloc((size_t)size * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter %d integers:\n", size);
    for (int i = 0; i < size; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            fprintf(stderr, "Invalid input for element\n");
            free(arr);
            return EXIT_FAILURE;
        }
    }
    
    int lower, upper;
    printf("Enter lower bound: ");
    if (scanf("%d", &lower) != 1) {
        fprintf(stderr, "Invalid input for lower bound\n");
        free(arr);
        return EXIT_FAILURE;
    }
    
    printf("Enter upper bound: ");
    if (scanf("%d", &upper) != 1) {
        fprintf(stderr, "Invalid input for upper bound\n");
        free(arr);
        return EXIT_FAILURE;
    }
    
    int result = count_elements_in_range(arr, size, lower, upper);
    if (result >= 0) {
        printf("Number of elements in range [%d, %d]: %d\n", lower, upper, result);
    } else {
        printf("Invalid range or parameters\n");
    }
    
    free(arr);
    return EXIT_SUCCESS;
}