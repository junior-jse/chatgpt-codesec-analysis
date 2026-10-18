#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct DictionaryEntry {
    char *key;
    char *value;
    struct DictionaryEntry *next;
} DictionaryEntry;

typedef struct {
    DictionaryEntry *head;
} Dictionary;

static bool all_dictionaries_empty(const Dictionary *dictionaries, size_t count)
{
    size_t i;

    if (count > 0U && dictionaries == NULL) {
        return false;
    }

    for (i = 0U; i < count; ++i) {
        if (dictionaries[i].head != NULL) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    Dictionary dictionaries[3] = {
        { .head = NULL },
        { .head = NULL },
        { .head = NULL }
    };

    if (all_dictionaries_empty(dictionaries, 3U)) {
        if (puts("All dictionaries are empty.") == EOF) {
            return EXIT_FAILURE;
        }
    } else {
        if (puts("At least one dictionary is not empty.") == EOF) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}