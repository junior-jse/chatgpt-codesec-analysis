#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    /* Possible weaknesses found:
     *  struct member 'Dictionary::entries' is never used. [unusedStructMember]
     */
    const void *entries;
    size_t entry_count;
} Dictionary;

bool all_dictionaries_empty(const Dictionary *dictionaries, size_t count)
{
    if (count == 0U) {
        return true;
    }

    if (dictionaries == NULL) {
        return false;
    }

    for (size_t i = 0U; i < count; ++i) {
        if (dictionaries[i].entry_count != 0U) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    const Dictionary dictionaries[] = {
        {NULL, 0U},
        {NULL, 0U},
        {NULL, 0U}
    };
    const size_t count = sizeof dictionaries / sizeof dictionaries[0];

    if (all_dictionaries_empty(dictionaries, count)) {
        if (puts("All dictionaries are empty.") == EOF) {
            return 1;
        }
    } else {
        if (puts("Not all dictionaries are empty.") == EOF) {
            return 1;
        }
    }

    return 0;
}