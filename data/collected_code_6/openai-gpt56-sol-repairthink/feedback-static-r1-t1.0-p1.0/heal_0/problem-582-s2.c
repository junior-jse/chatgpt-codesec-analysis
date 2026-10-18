#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    const char *key;
    const char *value;
} DictionaryEntry;

typedef struct {
    DictionaryEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

static bool dictionary_is_empty(const Dictionary *dictionary)
{
    return dictionary == NULL || dictionary->size == 0U;
}

int main(void)
{
    Dictionary dictionary = {
        .entries = NULL,
        .size = 0U,
        .capacity = 0U
    };

    if (dictionary_is_empty(&dictionary)) {
        puts("The dictionary is empty.");
    } else {
        puts("The dictionary is not empty.");
    }

    return 0;
}