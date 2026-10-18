#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    /* Possible weaknesses found:
     *  struct member 'DictEntry::value' is never used. [unusedStructMember]
     */
    char *value;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
    /* Possible weaknesses found:
     *  struct member 'Dictionary::capacity' is never used. [unusedStructMember]
     */
    size_t capacity;
} Dictionary;

char **dict_keys(const Dictionary *dict, size_t *count) {
    char **keys;
    size_t i;

    if (dict == NULL || count == NULL || dict->size == 0) {
        if (count != NULL) {
            *count = 0;
        }
        return NULL;
    }

    keys = malloc(dict->size * sizeof(char *));
    if (keys == NULL) {
        *count = 0;
        return NULL;
    }

    for (i = 0; i < dict->size; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        keys[i] = malloc(strlen(dict->entries[i].key) + 1);
        if (keys[i] == NULL) {
            size_t j;
            for (j = 0; j < i; j++) {
                free(keys[j]);
            }
            free(keys);
            *count = 0;
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
         */
        strcpy(keys[i], dict->entries[i].key);
    }

    *count = dict->size;
    return keys;
}

void free_keys(char **keys, size_t count) {
    size_t i;
    if (keys == NULL) {
        return;
    }
    for (i = 0; i < count; i++) {
        free(keys[i]);
    }
    free(keys);
}

int main(void) {
    DictEntry entries[] = {
        {"name", "Alice"},
        {"age", "30"},
        {"city", "New York"}
    };
    Dictionary dict = {entries, 3, 3};
    char **keys;
    size_t count;
    size_t i;

    keys = dict_keys(&dict, &count);
    if (keys == NULL) {
        fprintf(stderr, "Failed to retrieve dictionary keys\n");
        return EXIT_FAILURE;
    }

    printf("Dictionary keys:\n");
    for (i = 0; i < count; i++) {
        printf("%s\n", keys[i]);
    }

    free_keys(keys, count);
    return EXIT_SUCCESS;
}