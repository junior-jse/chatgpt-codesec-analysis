#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
} Dictionary;

Dictionary tuple_to_dict(char **tuple, size_t tuple_size) {
    Dictionary dict;
    dict.size = tuple_size / 2;
    dict.entries = NULL;

    if (dict.size == 0) {
        return dict;
    }

    dict.entries = (DictEntry *)malloc(dict.size * sizeof(DictEntry));
    if (dict.entries == NULL) {
        dict.size = 0;
        return dict;
    }

    for (size_t i = 0; i < dict.size; i++) {
        dict.entries[i].key = (char *)malloc(strlen(tuple[i * 2]) + 1);
        if (dict.entries[i].key == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(dict.entries[j].key);
                free(dict.entries[j].value);
            }
            free(dict.entries);
            dict.entries = NULL;
            dict.size = 0;
            return dict;
        }
        strcpy(dict.entries[i].key, tuple[i * 2]);

        dict.entries[i].value = (char *)malloc(strlen(tuple[i * 2 + 1]) + 1);
        if (dict.entries[i].value == NULL) {
            free(dict.entries[i].key);
            for (size_t j = 0; j < i; j++) {
                free(dict.entries[j].key);
                free(dict.entries[j].value);
            }
            free(dict.entries);
            dict.entries = NULL;
            dict.size = 0;
            return dict;
        }
        strcpy(dict.entries[i].value, tuple[i * 2 + 1]);
    }

    return dict;
}

void free_dictionary(Dictionary *dict) {
    if (dict == NULL || dict->entries == NULL) {
        return;
    }

    for (size_t i = 0; i < dict->size; i++) {
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

    Dictionary dict = tuple_to_dict(tuple, tuple_size);
    if (dict.entries == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return 1;
    }

    for (size_t i = 0; i < dict.size; i++) {
        printf("%s: %s\n", dict.entries[i].key, dict.entries[i].value);
    }

    free_dictionary(&dict);

    return 0;
}