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

    const char *first_value = dict->entries[0].value;

    for (int i = 1; i < dict->count; i++) {
        if (strcmp(first_value, dict->entries[i].value) != 0) {
            return false;
        }
    }

    return true;
}

int main(void) {
    Dictionary dict;
    dict.count = 0;

    snprintf(dict.entries[dict.count].key, MAX_KEY_LENGTH, "%s", "first");
    snprintf(dict.entries[dict.count].value, MAX_VALUE_LENGTH, "%s", "apple");
    dict.count++;

    snprintf(dict.entries[dict.count].key, MAX_KEY_LENGTH, "%s", "second");
    snprintf(dict.entries[dict.count].value, MAX_VALUE_LENGTH, "%s", "apple");
    dict.count++;

    snprintf(dict.entries[dict.count].key, MAX_KEY_LENGTH, "%s", "third");
    snprintf(dict.entries[dict.count].value, MAX_VALUE_LENGTH, "%s", "apple");
    dict.count++;

    if (all_values_same(&dict)) {
        printf("All values are same\n");
    } else {
        printf("Values are different\n");
    }

    snprintf(dict.entries[dict.count].key, MAX_KEY_LENGTH, "%s", "fourth");
    snprintf(dict.entries[dict.count].value, MAX_VALUE_LENGTH, "%s", "orange");
    dict.count++;

    if (all_values_same(&dict)) {
        printf("All values are same\n");
    } else {
        printf("Values are different\n");
    }

    return 0;
}