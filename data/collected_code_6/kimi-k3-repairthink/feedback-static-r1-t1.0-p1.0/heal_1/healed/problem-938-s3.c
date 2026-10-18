#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void findClosestElements(const int *arr1, int size1, const int *arr2, int size2, const int *arr3, int size3, int *result)
{
    if (arr1 == NULL || arr2 == NULL || arr3 == NULL || result == NULL)
    {
        return;
    }

    if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    {
        return;
    }

    int i = 0, j = 0, k = 0;
    int minDiff = INT_MAX;

    result[0] = arr1[0];
    result[1] = arr2[0];
    result[2] = arr3[0];

    while (i < size1 && j < size2 && k < size3)
    {
        int current1 = arr1[i];
        int current2 = arr2[j];
        int current3 = arr3[k];

        int maxVal = current1;
        if (current2 > maxVal)
        {
            maxVal = current2;
        }
        if (current3 > maxVal)
        {
            maxVal = current3;
        }

        int minVal = current1;
        if (current2 < minVal)
        {
            minVal = current2;
        }
        if (current3 < minVal)
        {
            minVal = current3;
        }

        int currentDiff = maxVal - minVal;

        if (currentDiff < minDiff)
        {
            minDiff = currentDiff;
            result[0] = current1;
            result[1] = current2;
            result[2] = current3;
        }

        if (currentDiff == 0)
        {
            break;
        }

        if (current1 == minVal)
        {
            i++;
        }
        else if (current2 == minVal)
        {
            j++;
        }
        else
        {
            k++;
        }
    }
}

int main(void)
{
    int arr1[] = {1, 4, 10};
    int arr2[] = {2, 15, 20};
    int arr3[] = {10, 12};

    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    int size3 = sizeof(arr3) / sizeof(arr3[0]);

    int *result = malloc(3 * sizeof(int));

    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    findClosestElements(arr1, size1, arr2, size2, arr3, size3, result);

    printf("Closest elements: %d %d %d\n", result[0], result[1], result[2]);

    free(result);
    result = NULL;

    return EXIT_SUCCESS;
}