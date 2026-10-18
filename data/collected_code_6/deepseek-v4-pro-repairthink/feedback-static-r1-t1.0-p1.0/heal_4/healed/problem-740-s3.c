#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 4096

typedef struct {
    char *key;
    char *value;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
} Dictionary;

static size_t safe_strlen(const char *str, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && str[i] != '\0'; i++);
    return i;
}

Dictionary tuple_to_dict(char **tuple, size_t tuple_size) {
    Dictionary dict;
    size_t i, j;

    dict.size = tuple_size / 2;
    dict.entries = NULL;

    if (dict.size == 0 || tuple_size % 2 != 0) {
        return dict;
    }

    dict.entries = (DictEntry *)malloc(dict.size * sizeof(DictEntry));
    if (dict.entries == NULL) {
        dict.size = 0;
        return dict;
    }

    for (i = 0; i < dict.size; i++) {
        if (tuple[i * 2] == NULL) {
            for (j = 0; j < i; j++) {
                free(dict.entries[j].key);
                free(dict.entries[j].value);
            }
            free(dict.entries);
            dict.entries = NULL;
            dict.size = 0;
            return dict;
        }
        size_t key_len = safe_strlen(tuple[i * 2], MAX_STRING_LEN);
        if (key_len == MAX_STRING_LEN) {
            for (j = 0; j < i; j++) {
                free(dict.entries[j].key);
                free(dict.entries[j].value);
            }
            free(dict.entries);
            dict.entries = NULL;
            dict.size = 0;
            return dict;
        }

        dict.entries[i].key = (char *)malloc(key_len + 1);
        if (dict.entries[i].key == NULL) {
            for (j = 0; j < i; j++) {
                free(dict.entries[j].key);
                free(dict.entries[j].value);
            }
            free(dict.entries);
            dict.entries = NULL;
            dict.size = 0;
            return dict;
        }
        snprintf(dict.entries[i].key, key_len + 1, "%s", tuple[i * 2]);

        if (tuple[i * 2 + 1] == NULL) {
            free(dict.entries[i].key);
            for (j = 0; j < i; j++) {
                free(dict.entries[j].key);
                free(dict.entries[j].value);
            }
            free(dict.entries);
            dict.entries = NULL;
            dict.size = 0;
            return dict;
        }
        size_t value_len = safe_strlen(tuple[i * 2 + 1], MAX_STRING_LEN);
        if (value_len == MAX_STRING_LEN) {
            free(dict.entries[i].key);
            for (j = 0; j < i; j++) {
                free(dict.entries[j].key);
                free(dict.entries[j].value);
            }
            free(dict.entries);
            dict.entries = NULL;
            dict.size = 0;
            return dict;
        }

        dict.entries[i].value = (char *)malloc(value_len + 1);
        if (dict.entries[i].value == NULL) {
            free(dict.entries[i].key);
            for (j = 0; j < i; j++) {
                free(dict.entries[j].key);
                free(dict.entries[j].value);
            }
            free(dict.entries);
            dict.entries = NULL;
            dict.size = 0;
            return dict;
        }
        snprintf(dict.entries[i].value, value_len + 1, "%s", tuple[i * 2 + 1]);
    }

    return dict;
}

void free_dictionary(Dictionary *dict) {
    size_t i;

    if (dict == NULL || dict->entries == NULL) {
        return;
    }

    for (i = 0; i < dict->size; i++) {
        free(dict->entries[i].key);
        free(dict->entries[i].value);
    }
    free(dict->entries);
    dict->entries = NULL;
    dict->size = 0;
}

int main(void) {
    char *tuple[] = {"name", "Alice", "age", "30", "city", "New York"};
    size_t tuple_size = sizeof(tuple) / sizeof(tuple[0]);
    Dictionary dict;
    size_t i;

    dict = tuple_to_dict(tuple, tuple_size);
    if (dict.entries == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return 1;
    }

    for (i = 0; i < dict.size; i++) {
        printf("%s: %s\n", dict.entries[i].key, dict.entries[i].value);
    }

    free_dictionary(&dict);

    return 0;
}