#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ENTRIES 100

typedef struct {
    char *key;
    char *value;
} DictionaryEntry;

typedef struct {
    DictionaryEntry entries[MAX_ENTRIES];
    int count;
} Dictionary;

bool all_values_same(const Dictionary *dict) {
    if (dict == NULL || dict->count == 0) {
        return true;
    }

    const char *first_value = dict->entries[0].value;

    for (int i = 1; i < dict->count; i++) {
        if (strcmp(first_value, dict->entries[i].value) != 0) {
            return false;
        }
    }

    return true;
}

void clear_dictionary(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    for (int i = 0; i < dict->count; i++) {
        free(dict->entries[i].key);
        free(dict->entries[i].value);
        dict->entries[i].key = NULL;
        dict->entries[i].value = NULL;
    }
    dict->count = 0;
}

bool add_entry(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return false;
    }
    if (dict->count >= MAX_ENTRIES) {
        return false;
    }

    size_t key_len = strlen(key);
    char *key_copy = malloc(key_len + 1);
    if (key_copy == NULL) {
        return false;
    }
    memcpy(key_copy, key, key_len + 1);

    size_t value_len = strlen(value);
    char *value_copy = malloc(value_len + 1);
    if (value_copy == NULL) {
        free(key_copy);
        return false;
    }
    memcpy(value_copy, value, value_len + 1);

    dict->entries[dict->count].key = key_copy;
    dict->entries[dict->count].value = value_copy;
    dict->count++;
    return true;
}

int main(void) {
    Dictionary dict;
    dict.count = 0;

    if (!add_entry(&dict, "first", "apple")) {
        printf("Error adding entry\n");
        clear_dictionary(&dict);
        return 1;
    }
    if (!add_entry(&dict, "second", "apple")) {
        printf("Error adding entry\n");
        clear_dictionary(&dict);
        return 1;
    }
    if (!add_entry(&dict, "third", "apple")) {
        printf("Error adding entry\n");
        clear_dictionary(&dict);
        return 1;
    }

    if (all_values_same(&dict)) {
        printf("All values are same\n");
    } else {
        printf("Values are different\n");
    }

    if (!add_entry(&dict, "fourth", "orange")) {
        printf("Error adding entry\n");
        clear_dictionary(&dict);
        return 1;
    }

    if (all_values_same(&dict)) {
        printf("All values are same\n");
    } else {
        printf("Values are different\n");
    }

    clear_dictionary(&dict);
    return 0;
}