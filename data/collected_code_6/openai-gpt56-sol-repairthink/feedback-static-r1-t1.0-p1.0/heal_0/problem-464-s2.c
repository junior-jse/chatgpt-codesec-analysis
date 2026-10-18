#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    const char *key;
    int value;
} DictionaryEntry;

static bool all_values_same(const DictionaryEntry *dictionary, size_t count)
{
    size_t i;

    if (count == 0U) {
        return true;
    }

    if (dictionary == NULL) {
        return false;
    }

    for (i = 1U; i < count; ++i) {
        if (dictionary[i].value != dictionary[0].value) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    const DictionaryEntry dictionary[] = {
        {"first", 42},
        {"second", 42},
        {"third", 42}
    };
    const size_t count = sizeof(dictionary) / sizeof(dictionary[0]);

    if (printf("%s\n", all_values_same(dictionary, count) ? "true" : "false") < 0) {
        return 1;
    }

    return 0;
}