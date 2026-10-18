#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

int extract_rear_elements(const Tuple *records, size_t count, int **result)
{
    size_t i;
    int *rear_elements;

    if (result == NULL || (records == NULL && count != 0)) {
        return -1;
    }

    *result = NULL;

    if (count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*rear_elements)) {
        return -1;
    }

    rear_elements = malloc(count * sizeof(*rear_elements));
    if (rear_elements == NULL) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        rear_elements[i] = records[i].second;
    }

    *result = rear_elements;
    return 0;
}

int main(void)
{
    const Tuple records[] = {
        {1, 10},
        {2, 20},
        {3, 30},
        {4, 40}
    };
    const size_t count = sizeof(records) / sizeof(records[0]);
    int *rear_elements = NULL;
    size_t i;

    if (extract_rear_elements(records, count, &rear_elements) != 0) {
        fputs("Failed to extract rear elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < count; ++i) {
        printf("%d%s", rear_elements[i], i + 1 == count ? "\n" : " ");
    }

    free(rear_elements);
    return EXIT_SUCCESS;
}