#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_KEY_LENGTH 64
#define MAX_VALUE_LENGTH 64
#define MAX_ENTRIES 100

typedef struct {
    char key[MAX_KEY_LENGTH + 1];
    char value[MAX_VALUE_LENGTH + 1];
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

static size_t safe_strlen(const char *s, size_t maxlen) {
    size_t i = 0;
    while (i < maxlen && s[i] != '\0') {
        i++;
    }
    return i;
}

bool add_entry(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return false;
    }
    if (dict->count >= MAX_ENTRIES) {
        return false;
    }
    if (safe_strlen(key, MAX_KEY_LENGTH + 1) > MAX_KEY_LENGTH ||
        safe_strlen(value, MAX_VALUE_LENGTH + 1) > MAX_VALUE_LENGTH) {
        return false;
    }
    int key_ret = snprintf(dict->entries[dict->count].key, MAX_KEY_LENGTH + 1, "%s", key);
    if (key_ret < 0 || key_ret >= MAX_KEY_LENGTH + 1) {
        return false;
    }
    int val_ret = snprintf(dict->entries[dict->count].value, MAX_VALUE_LENGTH + 1, "%s", value);
    if (val_ret < 0 || val_ret >= MAX_VALUE_LENGTH + 1) {
        return false;
    }
    dict->count++;
    return true;
}

int main(void) {
    Dictionary dict;
    dict.count = 0;

    if (!add_entry(&dict, "first", "apple")) {
        printf("Error adding entry\n");
        return 1;
    }
    if (!add_entry(&dict, "second", "apple")) {
        printf("Error adding entry\n");
        return 1;
    }
    if (!add_entry(&dict, "third", "apple")) {
        printf("Error adding entry\n");
        return 1;
    }

    if (all_values_same(&dict)) {
        printf("All values are same\n");
    } else {
        printf("Values are different\n");
    }

    if (!add_entry(&dict, "fourth", "orange")) {
        printf("Error adding entry\n");
        return 1;
    }

    if (all_values_same(&dict)) {
        printf("All values are same\n");
    } else {
        printf("Values are different\n");
    }

    return 0;
}