#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    /* Possible weaknesses found:
     *  struct member 'DictionaryEntry::key' is never used. [unusedStructMember]
     */
    const char *key;
    const char *value;
} DictionaryEntry;

static bool all_values_are_same(const DictionaryEntry *dictionary, size_t count)
{
    size_t i;

    if (dictionary == NULL || count == 0 || dictionary[0].value == NULL) {
        return false;
    }

    for (i = 1; i < count; ++i) {
        if (dictionary[i].value == NULL ||
            strcmp(dictionary[0].value, dictionary[i].value) != 0) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    const DictionaryEntry dictionary[] = {
        {"first", "value"},
        {"second", "value"},
        {"third", "value"}
    };
    const size_t count = sizeof(dictionary) / sizeof(dictionary[0]);

    if (all_values_are_same(dictionary, count)) {
        if (puts("All values are the same.") == EOF) {
            return 1;
        }
    } else {
        if (puts("Values are not all the same.") == EOF) {
            return 1;
        }
    }

    return 0;
}