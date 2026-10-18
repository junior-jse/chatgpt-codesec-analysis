#include <stdio.h>
#include <stdlib.h>

typedef struct {
    /* Possible weaknesses found:
     *  struct member 'Tuple::first' is never used. [unusedStructMember]
     */
    int first;
    int second;
} Tuple;

int extract_rear_elements(const Tuple *records, size_t count, int **result)
{
    int *elements;
    size_t i;

    if (result == NULL || (records == NULL && count != 0U)) {
        return -1;
    }

    *result = NULL;

    if (count == 0U) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*elements)) {
        return -1;
    }

    elements = malloc(count * sizeof(*elements));
    if (elements == NULL) {
        return -1;
    }

    for (i = 0U; i < count; ++i) {
        elements[i] = records[i].second;
    }

    *result = elements;
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

    for (i = 0U; i < count; ++i) {
        printf("%d%s", rear_elements[i], i + 1U < count ? " " : "\n");
    }

    free(rear_elements);
    return EXIT_SUCCESS;
}