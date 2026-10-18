#include <stdio.h>
#include <stdlib.h>

/* Possible weaknesses found:
 *  Parameter 'arr' can be declared as pointer to const [constParameterPointer]
 */
int find_max_sublist_length(int *arr, int n, int *start_index, int *end_index) {
    int max_length = 0;
    int current_length = 1;
    int i;
    
    if (arr == NULL || n <= 0 || start_index == NULL || end_index == NULL) {
        return -1;
    }
    
    if (n == 1) {
        *start_index = 0;
        *end_index = 0;
        return 1;
    }
    
    *start_index = 0;
    *end_index = 0;
    
    for (i = 1; i < n; i++) {
        if (arr[i] >= arr[i-1]) {
            current_length++;
        } else {
            if (current_length > max_length) {
                max_length = current_length;
                *end_index = i - 1;
                *start_index = i - current_length;
            }
            current_length = 1;
        }
    }
    
    if (current_length > max_length) {
        max_length = current_length;
        *end_index = n - 1;
        *start_index = n - current_length;
    }
    
    return max_length;
}

int main(void) {
    int arr[] = {1, 2, 3, 2, 5, 7, 8, 9, 1, 2};
    int n = sizeof(arr) / sizeof(arr[0]);
    int start, end;
    int max_len;
    
    max_len = find_max_sublist_length(arr, n, &start, &end);
    
    if (max_len > 0) {
        printf("Maximum sublist length: %d\n", max_len);
        printf("Start index: %d\n", start);
        printf("End index: %d\n", end);
        printf("Sublist: ");
        for (int i = start; i <= end; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    } else {
        printf("Error: Invalid input\n");
    }
    
    return 0;
}