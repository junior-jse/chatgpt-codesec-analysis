#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LENGTH 4096

typedef struct {
    char *key;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
} Dictionary;

char **dict_keys(const Dictionary *dict, size_t *count) {
    char **keys;
    size_t i;

    if (count == NULL) {
        return NULL;
    }

    *count = 0;

    if (dict == NULL || dict->entries == NULL || dict->size == 0) {
        return NULL;
    }

    keys = malloc(dict->size * sizeof(char *));
    if (keys == NULL) {
        return NULL;
    }

    for (i = 0; i < dict->size; i++) {
        size_t key_len;

        if (dict->entries[i].key == NULL) {
            size_t j;
            for (j = 0; j < i; j++) {
                free(keys[j]);
            }
            free(keys);
            return NULL;
        }

        key_len = strnlen(dict->entries[i].key, MAX_KEY_LENGTH + 1);
        if (key_len > MAX_KEY_LENGTH) {
            size_t j;
            for (j = 0; j < i; j++) {
                free(keys[j]);
            }
            free(keys);
            return NULL;
        }

        keys[i] = malloc(key_len + 1);
        if (keys[i] == NULL) {
            size_t j;
            for (j = 0; j < i; j++) {
                free(keys[j]);
            }
            free(keys);
            return NULL;
        }

        memcpy(keys[i], dict->entries[i].key, key_len);
        keys[i][key_len] = '\0';
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
        {"name"},
        {"age"},
        {"city"}
    };
    Dictionary dict = {entries, 3};
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