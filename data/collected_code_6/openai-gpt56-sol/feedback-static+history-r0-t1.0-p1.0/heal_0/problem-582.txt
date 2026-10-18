#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Dictionary {
    size_t size;
} Dictionary;

static bool dictionary_is_empty(const Dictionary *dictionary, bool *is_empty)
{
    if (dictionary == NULL || is_empty == NULL) {
        return false;
    }

    *is_empty = (dictionary->size == 0U);
    return true;
}

int main(void)
{
    Dictionary *dictionary = calloc(1U, sizeof(*dictionary));
    bool is_empty = false;

    if (dictionary == NULL) {
        fputs("Failed to allocate dictionary\n", stderr);
        return EXIT_FAILURE;
    }

    if (!dictionary_is_empty(dictionary, &is_empty)) {
        fputs("Failed to check dictionary\n", stderr);
        free(dictionary);
        return EXIT_FAILURE;
    }

    puts(is_empty ? "Dictionary is empty" : "Dictionary is not empty");

    free(dictionary);
    return EXIT_SUCCESS;
}