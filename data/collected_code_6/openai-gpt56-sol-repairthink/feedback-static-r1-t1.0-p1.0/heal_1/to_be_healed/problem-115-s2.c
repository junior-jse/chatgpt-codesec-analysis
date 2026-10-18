#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    /* Possible weaknesses found:
     *  struct member 'DictionaryEntry::key' is never used. [unusedStructMember]
     */
    char *key;
    /* Possible weaknesses found:
     *  struct member 'DictionaryEntry::value' is never used. [unusedStructMember]
     */
    char *value;
} DictionaryEntry;

typedef struct {
    /* Possible weaknesses found:
     *  struct member 'Dictionary::entries' is never used. [unusedStructMember]
     */
    DictionaryEntry *entries;
    size_t count;
} Dictionary;

static bool all_dictionaries_empty(const Dictionary *dictionaries, size_t count)
{
    size_t i;

    if (count > 0U && dictionaries == NULL) {
        return false;
    }

    for (i = 0U; i < count; ++i) {
        if (dictionaries[i].count != 0U) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    Dictionary dictionaries[3] = {
        {NULL, 0U},
        {NULL, 0U},
        {NULL, 0U}
    };

    if (all_dictionaries_empty(dictionaries,
                               sizeof dictionaries / sizeof dictionaries[0])) {
        puts("All dictionaries are empty.");
    } else {
        puts("At least one dictionary is not empty.");
    }

    return EXIT_SUCCESS;
}