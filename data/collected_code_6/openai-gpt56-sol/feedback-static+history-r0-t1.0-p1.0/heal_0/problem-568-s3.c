#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void *entries;
    size_t count;
} Dictionary;

static Dictionary *create_empty_dictionaries(size_t count)
{
    Dictionary *dictionaries;

    if (count == 0) {
        return NULL;
    }

    if (count > SIZE_MAX / sizeof(*dictionaries)) {
        return NULL;
    }

    dictionaries = calloc(count, sizeof(*dictionaries));
    return dictionaries;
}

static void destroy_dictionaries(Dictionary *dictionaries)
{
    free(dictionaries);
}

int main(void)
{
    const size_t count = 10;
    Dictionary *dictionaries = create_empty_dictionaries(count);

    if (dictionaries == NULL) {
        fputs("Failed to create dictionaries\n", stderr);
        return EXIT_FAILURE;
    }

    printf("Created %zu empty dictionaries\n", count);

    destroy_dictionaries(dictionaries);
    return EXIT_SUCCESS;
}