#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void *entries;
    size_t size;
} Dictionary;

Dictionary *create_empty_dictionaries(size_t count)
{
    Dictionary *list;

    if (count == 0) {
        return NULL;
    }

    if (count > SIZE_MAX / sizeof(*list)) {
        return NULL;
    }

    list = calloc(count, sizeof(*list));
    return list;
}

int main(void)
{
    const size_t count = 5;
    Dictionary *dictionaries = create_empty_dictionaries(count);

    if (dictionaries == NULL) {
        fprintf(stderr, "Failed to create dictionary list\n");
        return EXIT_FAILURE;
    }

    free(dictionaries);
    dictionaries = NULL;

    return EXIT_SUCCESS;
}