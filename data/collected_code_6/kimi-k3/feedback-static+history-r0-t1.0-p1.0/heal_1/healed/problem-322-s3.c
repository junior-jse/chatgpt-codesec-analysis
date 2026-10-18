#include <stdio.h>
#include <stdlib.h>

int *find_min_indices(const int *arr, size_t size, size_t *count)
{
    if (arr == NULL || count == NULL || size == 0)
    {
        if (count != NULL)
        {
            *count = 0;
        }
        return NULL;
    }

    int min_val = arr[0];
    size_t min_count = 0;
    int *indices = NULL;

    for (size_t i = 1; i < size; i++)
    {
        if (arr[i] < min_val)
        {
            min_val = arr[i];
        }
    }

    for (size_t i = 0; i < size; i++)
    {
        if (arr[i] == min_val)
        {
            min_count++;
        }
    }

    indices = (int *)malloc(min_count * sizeof(int));
    if (indices == NULL)
    {
        *count = 0;
        return NULL;
    }

    min_count = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (arr[i] == min_val)
        {
            indices[min_count] = (int)i;
            min_count++;
        }
    }

    *count = min_count;
    return indices;
}

int main(void)
{
    int arr[] = {5, 2, 8, 2, 9, 1, 1, 7};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    size_t count = 0;
    int *result = NULL;

    result = find_min_indices(arr, size, &count);

    if (result != NULL)
    {
        printf("Minimum value indices: ");
        for (size_t i = 0; i < count; i++)
        {
            printf("%d ", result[i]);
        }
        printf("\n");
        printf("Total occurrences: %zu\n", count);
        free(result);
        result = NULL;
    }
    else
    {
        printf("Error: Unable to find minimum indices.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}