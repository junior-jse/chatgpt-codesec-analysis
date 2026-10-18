#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *key;
    char *value;
} DictionaryEntry;

typedef struct {
    DictionaryEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

static bool dictionary_is_empty(const Dictionary *dictionary, bool *is_empty)
{
    if (dictionary == NULL || is_empty == NULL) {
        return false;
    }

    *is_empty = dictionary->size == 0U;
    return true;
}

int main(void)
{
    Dictionary dictionary = {
        .entries = NULL,
        .size = 0U,
        .capacity = 0U
    };
    bool is_empty = false;

    if (!dictionary_is_empty(&dictionary, &is_empty)) {
        (void)fprintf(stderr, "Failed to check dictionary state\n");
        return EXIT_FAILURE;
    }

    (void)printf("Dictionary is %s.\n", is_empty ? "empty" : "not empty");

    free(dictionary.entries);
    dictionary.entries = NULL;
    dictionary.size = 0U;
    dictionary.capacity = 0U;

    return EXIT_SUCCESS;
}