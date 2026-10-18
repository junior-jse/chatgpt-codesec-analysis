#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char *key;
    char *value;
} DictionaryEntry;

typedef struct {
    DictionaryEntry *entries;
    size_t size;
} Dictionary;

static bool dictionary_contains(const Dictionary *dictionary, const char *key)
{
    if (dictionary == NULL || key == NULL) {
        return false;
    }

    for (size_t i = 0; i < dictionary->size; ++i) {
        if (dictionary->entries[i].key != NULL &&
            strcmp(dictionary->entries[i].key, key) == 0) {
            return true;
        }
    }

    return false;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <key>\n", argv[0]);
        return EXIT_FAILURE;
    }

    DictionaryEntry entries[] = {
        {"name", "Alice"},
        {"age", "30"},
        {"city", "London"}
    };

    Dictionary dictionary = {
        entries,
        sizeof(entries) / sizeof(entries[0])
    };

    printf("%s\n", dictionary_contains(&dictionary, argv[1])
        ? "Key is present"
        : "Key is not present");

    return EXIT_SUCCESS;
}