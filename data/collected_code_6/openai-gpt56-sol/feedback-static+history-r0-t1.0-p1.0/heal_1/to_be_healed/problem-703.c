#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define DICTIONARY_SIZE 5U

typedef struct {
    const char *key;
    /* Possible weaknesses found:
     *  struct member 'DictionaryEntry::value' is never used. [unusedStructMember]
     */
    const char *value;
} DictionaryEntry;

static bool dictionary_contains(const DictionaryEntry *dictionary,
                                size_t dictionary_size,
                                const char *key)
{
    size_t i;

    if (dictionary == NULL || key == NULL) {
        return false;
    }

    for (i = 0U; i < dictionary_size; ++i) {
        if (dictionary[i].key != NULL &&
            strcmp(dictionary[i].key, key) == 0) {
            return true;
        }
    }

    return false;
}

int main(void)
{
    const DictionaryEntry dictionary[DICTIONARY_SIZE] = {
        {"apple", "fruit"},
        {"carrot", "vegetable"},
        {"blue", "color"},
        {"dog", "animal"},
        {"table", "furniture"}
    };
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char key[128];

    if (fgets(key, sizeof key, stdin) == NULL) {
        if (ferror(stdin) != 0) {
            fprintf(stderr, "Failed to read input.\n");
            return 1;
        }
        fprintf(stderr, "No input provided.\n");
        return 1;
    }

    key[strcspn(key, "\r\n")] = '\0';

    if (key[0] == '\0') {
        fprintf(stderr, "Key must not be empty.\n");
        return 1;
    }

    if (dictionary_contains(dictionary, DICTIONARY_SIZE, key)) {
        puts("Key is present.");
    } else {
        puts("Key is not present.");
    }

    return 0;
}