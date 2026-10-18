#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *key;
    const char *value;
} DictionaryEntry;

static bool dictionary_contains(const DictionaryEntry *dictionary,
                                size_t entry_count,
                                const char *key)
{
    if (dictionary == NULL || key == NULL) {
        return false;
    }

    for (size_t i = 0; i < entry_count; ++i) {
        if (dictionary[i].key != NULL && strcmp(dictionary[i].key, key) == 0) {
            return true;
        }
    }

    return false;
}

int main(int argc, char *argv[])
{
    static const DictionaryEntry dictionary[] = {
        {"apple", "fruit"},
        {"carrot", "vegetable"},
        {"salmon", "fish"}
    };
    const size_t entry_count = sizeof dictionary / sizeof dictionary[0];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <key>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (dictionary_contains(dictionary, entry_count, argv[1])) {
        printf("Key \"%s\" is present.\n", argv[1]);
    } else {
        printf("Key \"%s\" is not present.\n", argv[1]);
    }

    return EXIT_SUCCESS;
}