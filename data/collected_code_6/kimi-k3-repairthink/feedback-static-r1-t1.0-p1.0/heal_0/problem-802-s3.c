#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int countRotations(const int arr[], int n)
{
    if (arr == NULL || n <= 0)
    {
        return -1;
    }
    
    if (n == 1)
    {
        return 0;
    }
    
    int min_val = arr[0];
    int min_index = 0;
    
    for (int i = 1; i < n; i++)
    {
        if (arr[i] < min_val)
        {
            min_val = arr[i];
            min_index = i;
        }
    }
    
    return min_index;
}

int main(void)
{
    int arr1[] = {15, 18, 2, 3, 6, 12};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int result1 = countRotations(arr1, n1);
    
    if (result1 >= 0)
    {
        printf("Array 1 rotations: %d\n", result1);
    }
    else
    {
        fprintf(stderr, "Error processing array 1\n");
    }
    
    int arr2[] = {7, 9, 11, 12, 5};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    int result2 = countRotations(arr2, n2);
    
    if (result2 >= 0)
    {
        printf("Array 2 rotations: %d\n", result2);
    }
    else
    {
        fprintf(stderr, "Error processing array 2\n");
    }
    
    int arr3[] = {1, 2, 3, 4, 5};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    int result3 = countRotations(arr3, n3);
    
    if (result3 >= 0)
    {
        printf("Array 3 rotations: %d\n", result3);
    }
    else
    {
        fprintf(stderr, "Error processing array 3\n");
    }
    
    int arr4[] = {30};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    int result4 = countRotations(arr4, n4);
    
    if (result4 >= 0)
    {
        printf("Array 4 rotations: %d\n", result4);
    }
    else
    {
        fprintf(stderr, "Error processing array 4\n");
    }
    
    int result5 = countRotations(NULL, 5);
    if (result5 < 0)
    {
        printf("NULL array handled correctly\n");
    }
    
    return EXIT_SUCCESS;
}