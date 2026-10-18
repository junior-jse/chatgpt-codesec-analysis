#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int count_in_range(const int *arr, size_t size, int min, int max, size_t *result)
{
    size_t i;
    size_t count;

    if (arr == NULL || result == NULL)
    {
        return -1;
    }

    if (min > max)
    {
        return -2;
    }

    count = 0;
    for (i = 0; i < size; i++)
    {
        if (arr[i] >= min && arr[i] <= max)
        {
            count++;
        }
    }

    *result = count;
    return 0;
}

int main(void)
{
    int data[] = {5, 12, 3, 8, 15, 20, 7, 10, 1, 18};
    size_t data_size;
    size_t result;
    int status;

    data_size = sizeof(data) / sizeof(data[0]);

    status = count_in_range(data, data_size, 5, 15, &result);

    if (status != 0)
    {
        fprintf(stderr, "Error: count_in_range failed with code %d\n", status);
        return EXIT_FAILURE;
    }

    printf("Number of elements in range: %zu\n", result);

    return EXIT_SUCCESS;
}