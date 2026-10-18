#include <stdio.h>
#include <stdlib.h>

typedef struct List {
    void *data;
    struct List *next;
} List;

size_t count_lists(const List *lists)
{
    size_t count = 0;

    while (lists != NULL) {
        ++count;
        lists = lists->next;
    }

    return count;
}

static void free_lists(List *lists)
{
    while (lists != NULL) {
        List *next = lists->next;
        free(lists);
        lists = next;
    }
}

int main(void)
{
    List *lists = NULL;

    for (size_t i = 0; i < 5; ++i) {
        List *node = malloc(sizeof(*node));

        if (node == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free_lists(lists);
            return EXIT_FAILURE;
        }

        node->data = NULL;
        node->next = lists;
        lists = node;
    }

    printf("Number of lists: %zu\n", count_lists(lists));

    free_lists(lists);
    return EXIT_SUCCESS;
}