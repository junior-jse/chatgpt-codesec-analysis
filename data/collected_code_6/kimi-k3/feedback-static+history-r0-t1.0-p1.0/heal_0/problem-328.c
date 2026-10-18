#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int rotate_left(int *arr, size_t n, size_t k) {
    if (arr == NULL || n == 0) {
        return -1;
    }
    
    k = k % n;
    if (k == 0) {
        return 0;
    }
    
    int *temp = malloc(k * sizeof(int));
    if (temp == NULL) {
        return -1;
    }
    
    memcpy(temp, arr, k * sizeof(int));
    memmove(arr, arr + k, (n - k) * sizeof(int));
    memcpy(arr + n - k, temp, k * sizeof(int));
    
    free(temp);
    return 0;
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    size_t n = sizeof(arr) / sizeof(arr[0]);
    size_t k = 3;
    
    if (rotate_left(arr, n, k) != 0) {
        fprintf(stderr, "Rotation failed\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return EXIT_SUCCESS;
}