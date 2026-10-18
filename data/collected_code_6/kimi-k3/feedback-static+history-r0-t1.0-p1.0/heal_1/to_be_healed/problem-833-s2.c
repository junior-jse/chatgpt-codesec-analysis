#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **keys;
    size_t count;
} KeyList;

typedef struct {
    char *key;
    /* Possible weaknesses found:
     *  struct member 'DictEntry::value' is never used. [unusedStructMember]
     */
    int value;
} DictEntry;

static KeyList get_dictionary_keys(const DictEntry *dict, size_t size) {
    KeyList list;
    list.count = 0;
    list.keys = NULL;

    if (dict == NULL || size == 0) {
        return list;
    }

    list.keys = malloc(size * sizeof(char *));
    if (list.keys == NULL) {
        return list;
    }

    for (size_t i = 0; i < size; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        list.keys[i] = malloc(strlen(dict[i].key) + 1);
        if (list.keys[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(list.keys[j]);
            }
            free(list.keys);
            list.keys = NULL;
            list.count = 0;
            return list;
        }
        /* Possible weaknesses found:
         * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
         */
        strcpy(list.keys[i], dict[i].key);
        list.count++;
    }

    return list;
}

static void free_key_list(KeyList *list) {
    if (list == NULL || list->keys == NULL) {
        return;
    }
    for (size_t i = 0; i < list->count; i++) {
        free(list->keys[i]);
    }
    free(list->keys);
    list->keys = NULL;
    list->count = 0;
}

int main(void) {
    DictEntry dict[] = {
        {"apple", 1},
        {"banana", 2},
        {"cherry", 3}
    };
    /* Possible weaknesses found:
     *  Assignment 'dict_size=sizeof(dict)/sizeof(dict[0])', assigned value is 3
     */
    size_t dict_size = sizeof(dict) / sizeof(dict[0]);

    KeyList keys = get_dictionary_keys(dict, dict_size);

    /* Possible weaknesses found:
     *  Condition 'dict_size>0' is always true [knownConditionTrueFalse]
     *  Condition 'dict_size>0' is always true
     */
    if (keys.keys == NULL && dict_size > 0) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < keys.count; i++) {
        printf("%s\n", keys.keys[i]);
    }

    free_key_list(&keys);

    return EXIT_SUCCESS;
}