#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_KEY_LENGTH 64
#define MAX_VALUE_LENGTH 64
#define MAX_ENTRIES 100

typedef struct {
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];
} DictionaryEntry;

typedef struct {
    DictionaryEntry entries[MAX_ENTRIES];
    int count;
} Dictionary;

bool all_values_same(const Dictionary *dict) {
    if (dict == NULL || dict->count == 0) {
        return true;
    }

    if (dict->count > MAX_ENTRIES) {
        return false;
    }

    const char *first_value = dict->entries[0].value;

    for (int i = 1; i < dict->count; i++) {
        if (strncmp(first_value, dict->entries[i].value, MAX_VALUE_LENGTH) != 0) {
            return false;
        }
    }

    return true;
}

static bool set_entry(DictionaryEntry *entry, const char *key, const char *value) {
    if (entry == NULL || key == NULL || value == NULL) {
        return false;
    }

    size_t key_len = strnlen(key, MAX_KEY_LENGTH - 1);
    size_t value_len = strnlen(value, MAX_VALUE_LENGTH - 1);
    
    if (key_len >= MAX_KEY_LENGTH) {
        key_len = MAX_KEY_LENGTH - 1;
    }
    if (value_len >= MAX_VALUE_LENGTH) {
        value_len = MAX_VALUE_LENGTH - 1;
    }
    
    if (key_len < MAX_KEY_LENGTH) {
        memcpy(entry->key, key, key_len);
        entry->key[key_len] = '\0';
    } else {
        memcpy(entry->key, key, MAX_KEY_LENGTH - 1);
        entry->key[MAX_KEY_LENGTH - 1] = '\0';
    }
    
    if (value_len < MAX_VALUE_LENGTH) {
        memcpy(entry->value, value, value_len);
        entry->value[value_len] = '\0';
    } else {
        memcpy(entry->value, value, MAX_VALUE_LENGTH - 1);
        entry->value[MAX_VALUE_LENGTH - 1] = '\0';
    }

    return true;
}

int main(void) {
    Dictionary dict;
    dict.count = 0;

    if (dict.count < MAX_ENTRIES) {
        set_entry(&dict.entries[dict.count], "first", "apple");
        dict.count++;
    }

    if (dict.count < MAX_ENTRIES) {
        set_entry(&dict.entries[dict.count], "second", "apple");
        dict.count++;
    }

    if (dict.count < MAX_ENTRIES) {
        set_entry(&dict.entries[dict.count], "third", "apple");
        dict.count++;
    }

    if (all_values_same(&dict)) {
        printf("All values are same\n");
    } else {
        printf("Values are different\n");
    }

    if (dict.count < MAX_ENTRIES) {
        set_entry(&dict.entries[dict.count], "fourth", "orange");
        dict.count++;
    }

    if (all_values_same(&dict)) {
        printf("All values are same\n");
    } else {
        printf("Values are different\n");
    }

    return 0;
}