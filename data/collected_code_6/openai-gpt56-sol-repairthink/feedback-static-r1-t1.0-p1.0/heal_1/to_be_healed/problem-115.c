#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct DictionaryEntry {
    /* Possible weaknesses found:
     *  struct member 'DictionaryEntry::key' is never used. [unusedStructMember]
     */
    char *key;
    /* Possible weaknesses found:
     *  struct member 'DictionaryEntry::value' is never used. [unusedStructMember]
     */
    char *value;
    /* Possible weaknesses found:
     *  struct member 'DictionaryEntry::next' is never used. [unusedStructMember]
     */
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
    /* Possible weaknesses found:
     *  Variable 'dictionaries' can be declared as const array [constVariable]
     */
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