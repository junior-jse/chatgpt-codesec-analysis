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

    strncpy(dict.entries[dict.count].key, "first", MAX_KEY_LENGTH - 1);
    dict.entries[dict.count].key[MAX_KEY_LENGTH - 1] = '\0';
    strncpy(dict.entries[dict.count].value, "apple", MAX_VALUE_LENGTH - 1);
    dict.entries[dict.count].value[MAX_VALUE_LENGTH - 1] = '\0';
    dict.count++;

    strncpy(dict.entries[dict.count].key, "second", MAX_KEY_LENGTH - 1);
    dict.entries[dict.count].key[MAX_KEY_LENGTH - 1] = '\0';
    strncpy(dict.entries[dict.count].value, "apple", MAX_VALUE_LENGTH - 1);
    dict.entries[dict.count].value[MAX_VALUE_LENGTH - 1] = '\0';
    dict.count++;

    strncpy(dict.entries[dict.count].key, "third", MAX_KEY_LENGTH - 1);
    dict.entries[dict.count].key[MAX_KEY_LENGTH - 1] = '\0';
    strncpy(dict.entries[dict.count].value, "apple", MAX_VALUE_LENGTH - 1);
    dict.entries[dict.count].value[MAX_VALUE_LENGTH - 1] = '\0';
    dict.count++;

    if (all_values_same(&dict)) {
        printf("All values are same\n");
    } else {
        printf("Values are different\n");
    }

    strncpy(dict.entries[dict.count].key, "fourth", MAX_KEY_LENGTH - 1);
    dict.entries[dict.count].key[MAX_KEY_LENGTH - 1] = '\0';
    strncpy(dict.entries[dict.count].value, "orange", MAX_VALUE_LENGTH - 1);
    dict.entries[dict.count].value[MAX_VALUE_LENGTH - 1] = '\0';
    dict.count++;

    if (all_values_same(&dict)) {
        printf("All values are same\n");
    } else {
        printf("Values are different\n");
    }

    return 0;
}