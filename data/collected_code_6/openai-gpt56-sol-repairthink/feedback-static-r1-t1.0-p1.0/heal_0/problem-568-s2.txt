#include <stdio.h>
#include <stdlib.h>

typedef struct Dictionary {
    void *entries;
    size_t size;
} Dictionary;

static void destroy_dictionary_list(Dictionary **list, size_t count)
{
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(list[i]);
    }

    free(list);
}

static Dictionary **create_empty_dictionary_list(size_t count)
{
    if (count == 0) {
        return NULL;
    }

    if (count > SIZE_MAX / sizeof(Dictionary *)) {
        return NULL;
    }

    Dictionary **list = calloc(count, sizeof(*list));
    if (list == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        list[i] = calloc(1, sizeof(*list[i]));
        if (list[i] == NULL) {
            destroy_dictionary_list(list, i);
            return NULL;
        }
    }

    return list;
}

int main(void)
{
    const size_t count = 5;
    Dictionary **list = create_empty_dictionary_list(count);

    if (list == NULL) {
        fputs("Failed to create dictionary list.\n", stderr);
        return EXIT_FAILURE;
    }

    printf("Created %zu empty dictionaries.\n", count);
    destroy_dictionary_list(list, count);

    return EXIT_SUCCESS;
}