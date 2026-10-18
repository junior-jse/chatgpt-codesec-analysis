#include <stdio.h>
#include <stdlib.h>

typedef struct List {
    /* Possible weaknesses found:
     *  struct member 'List::items' is never used. [unusedStructMember]
     */
    int *items;
    /* Possible weaknesses found:
     *  struct member 'List::length' is never used. [unusedStructMember]
     */
    size_t length;
} List;

size_t count_lists(const List *lists, size_t list_count)
{
    if (lists == NULL && list_count != 0U) {
        return 0U;
    }

    return list_count;
}

int main(void)
{
    List lists[] = {
        {NULL, 0U},
        {NULL, 0U},
        {NULL, 0U}
    };
    const size_t list_count = sizeof(lists) / sizeof(lists[0]);

    if (printf("%zu\n", count_lists(lists, list_count)) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}