#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} DictionaryEntry;

typedef struct {
    DictionaryEntry *entries;
    size_t size;
} Dictionary;

char **extract_unique_values(const Dictionary *dict, size_t *out_size) {
    if (dict == NULL || out_size == NULL || dict->entries == NULL) {
        if (out_size != NULL) {
            *out_size = 0;
        }
        return NULL;
    }

    char **unique_values = malloc(dict->size * sizeof(char *));
    if (unique_values == NULL) {
        *out_size = 0;
        return NULL;
    }

    size_t unique_count = 0;

    for (size_t i = 0; i < dict->size; i++) {
        if (dict->entries[i].value == NULL) {
            continue;
        }

        int is_duplicate = 0;
        for (size_t j = 0; j < unique_count; j++) {
            if (strcmp(dict->entries[i].value, unique_values[j]) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            unique_values[unique_count] = dict->entries[i].value;
            unique_count++;
        }
    }

    if (unique_count == 0) {
        free(unique_values);
        *out_size = 0;
        return NULL;
    }

    char **shrunk_array = realloc(unique_values, unique_count * sizeof(char *));
    if (shrunk_array != NULL) {
        unique_values = shrunk_array;
    }

    *out_size = unique_count;
    return unique_values;
}

void free_dictionary(Dictionary *dict) {
    if (dict == NULL) {
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
    Dictionary dict;
    dict.size = 6;
    dict.entries = malloc(dict.size * sizeof(DictionaryEntry));
    if (dict.entries == NULL) {
        return EXIT_FAILURE;
    }

    dict.entries[0].key = strdup("a");
    dict.entries[0].value = strdup("apple");
    dict.entries[1].key = strdup("b");
    dict.entries[1].value = strdup("banana");
    dict.entries[2].key = strdup("c");
    dict.entries[2].value = strdup("apple");
    dict.entries[3].key = strdup("d");
    dict.entries[3].value = strdup("cherry");
    dict.entries[4].key = strdup("e");
    dict.entries[4].value = strdup("banana");
    dict.entries[5].key = strdup("f");
    dict.entries[5].value = strdup("date");

    if (dict.entries[0].key == NULL || dict.entries[0].value == NULL ||
        dict.entries[1].key == NULL || dict.entries[1].value == NULL ||
        dict.entries[2].key == NULL || dict.entries[2].value == NULL ||
        dict.entries[3].key == NULL || dict.entries[3].value == NULL ||
        dict.entries[4].key == NULL || dict.entries[4].value == NULL ||
        dict.entries[5].key == NULL || dict.entries[5].value == NULL) {
        free_dictionary(&dict);
        return EXIT_FAILURE;
    }

    size_t unique_count = 0;
    char **unique_values = extract_unique_values(&dict, &unique_count);

    if (unique_values != NULL) {
        for (size_t i = 0; i < unique_count; i++) {
            printf("%s\n", unique_values[i]);
        }
        free(unique_values);
    }

    free_dictionary(&dict);

    return EXIT_SUCCESS;
}