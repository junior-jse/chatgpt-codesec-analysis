#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **keys;
    size_t count;
} KeyList;

typedef struct {
    char *key;
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
        if (dict[i].key == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(list.keys[j]);
            }
            free(list.keys);
            list.keys = NULL;
            list.count = 0;
            return list;
        }
        
        size_t key_len = strnlen(dict[i].key, 256);
        list.keys[i] = malloc(key_len + 1);
        if (list.keys[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(list.keys[j]);
            }
            free(list.keys);
            list.keys = NULL;
            list.count = 0;
            return list;
        }
        
        snprintf(list.keys[i], key_len + 1, "%s", dict[i].key);
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

static int get_dict_value(const DictEntry *dict, size_t size, const char *key) {
    if (dict == NULL || key == NULL) {
        return -1;
    }
    for (size_t i = 0; i < size; i++) {
        if (dict[i].key != NULL && strcmp(dict[i].key, key) == 0) {
            return dict[i].value;
        }
    }
    return -1;
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
        int value = get_dict_value(dict, dict_size, keys.keys[i]);
        printf("%s: %d\n", keys.keys[i], value);
    }

    free_key_list(&keys);

    return EXIT_SUCCESS;
}