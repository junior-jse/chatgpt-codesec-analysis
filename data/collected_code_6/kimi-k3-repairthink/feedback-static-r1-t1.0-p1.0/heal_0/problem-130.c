#include <stdio.h>
#include <stdlib.h>

int find_max_frequency(const int *arr, size_t size, int *max_item, size_t *max_freq)
{
    size_t i;
    size_t j;
    size_t count;
    size_t highest;

    if (arr == NULL || max_item == NULL || max_freq == NULL || size == 0U)
    {
        return -1;
    }

    highest = 0U;
    *max_item = arr[0];
    *max_freq = 0U;

    for (i = 0U; i < size; i++)
    {
        count = 0U;

        for (j = 0U; j < size; j++)
        {
            if (arr[i] == arr[j])
            {
                count++;
            }
        }

        if (count > highest)
        {
            highest = count;
            *max_item = arr[i];
            *max_freq = count;
        }
    }

    return 0;
}

int main(void)
{
    int items[] = {4, 2, 7, 2, 4, 4, 8, 2, 7};
    size_t size;
    int max_item;
    size_t max_freq;
    int result;

    size = sizeof(items) / sizeof(items[0]);
    max_item = 0;
    max_freq = 0U;

    result = find_max_frequency(items, size, &max_item, &max_freq);

    if (result != 0)
    {
        fprintf(stderr, "Error: invalid input\n");
        return EXIT_FAILURE;
    }

    printf("Item with maximum frequency: %d\n", max_item);
    printf("Frequency: %zu\n", max_freq);

    return EXIT_SUCCESS;
}