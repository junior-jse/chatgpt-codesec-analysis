#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_KEY_LENGTH 1024

typedef struct {
    char **keys;
    size_t count;
} KeyList;

typedef struct {
    const char *key;
} DictEntry;

static size_t bounded_strlen(const char *s, size_t max_length) {
    size_t length = 0;
    while (length < max_length && s[length] != '\0') {
        length++;
    }
    return length;
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

static bool get_dictionary_keys(const DictEntry *dict, size_t size, KeyList *list) {
    if (list == NULL) {
        return false;
    }

    list->keys = NULL;
    list->count = 0;

    if (dict == NULL || size == 0) {
        return true;
    }

    list->keys = malloc(size * sizeof(list->keys[0]));
    if (list->keys == NULL) {
        return false;
    }

    for (size_t i = 0; i < size; i++) {
        if (dict[i].key == NULL) {
            free_key_list(list);
            return false;
        }

        size_t key_length = bounded_strlen(dict[i].key, MAX_KEY_LENGTH);
        if (key_length == MAX_KEY_LENGTH) {
            free_key_list(list);
            return false;
        }

        list->keys[i] = malloc(key_length + 1);
        if (list->keys[i] == NULL) {
            free_key_list(list);
            return false;
        }

        memcpy(list->keys[i], dict[i].key, key_length + 1);
        list->count++;
    }

    return true;
}

int main(void) {
    const DictEntry dict[] = {
        {"apple"},
        {"banana"},
        {"cherry"}
    };
    const size_t dict_size = sizeof(dict) / sizeof(dict[0]);

    KeyList keys;
    if (!get_dictionary_keys(dict, dict_size, &keys)) {
        fprintf(stderr, "Failed to build dictionary key list\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < keys.count; i++) {
        printf("%s\n", keys.keys[i]);
    }

    free_key_list(&keys);

    return EXIT_SUCCESS;
}