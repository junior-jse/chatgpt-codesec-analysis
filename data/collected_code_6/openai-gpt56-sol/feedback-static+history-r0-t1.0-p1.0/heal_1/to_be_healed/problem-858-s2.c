#include <stdio.h>
#include <stdlib.h>

typedef struct List {
    /* Possible weaknesses found:
     *  struct member 'List::data' is never used. [unusedStructMember]
     */
    const void *data;
    struct List *next;
} List;

int square_list_count(const List *lists, size_t *result)
{
    size_t count = 0;

    if (result == NULL) {
        return -1;
    }

    while (lists != NULL) {
        if (count == SIZE_MAX) {
            return -1;
        }
        ++count;
        lists = lists->next;
    }

    if (count != 0 && count > SIZE_MAX / count) {
        return -1;
    }

    *result = count * count;
    return 0;
}

int main(void)
{
    List third = {NULL, NULL};
    List second = {NULL, &third};
    List first = {NULL, &second};
    size_t result = 0;

    if (square_list_count(&first, &result) != 0) {
        fputs("Unable to compute the squared count.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%zu\n", result) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}