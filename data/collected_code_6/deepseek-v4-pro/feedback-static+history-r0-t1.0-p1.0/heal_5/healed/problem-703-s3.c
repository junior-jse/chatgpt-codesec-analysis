#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define TABLE_SIZE 100
#define KEY_MAX_LENGTH 49

typedef struct Entry {
    char key[50];
    int value;
    struct Entry *next;
} Entry;

typedef struct Dictionary {
    Entry *buckets[TABLE_SIZE];
} Dictionary;

unsigned int hash(const char *key) {
    unsigned int hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

Dictionary *create_dictionary(void) {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        dict->buckets[i] = NULL;
    }
    return dict;
}

bool dictionary_contains(Dictionary *dict, const char *key) {
    if (dict == NULL || key == NULL) {
        return false;
    }
    
    unsigned int index = hash(key);
    Entry *current = dict->buckets[index];
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return true;
        }
        current = current->next;
    }
    
    return false;
}

bool dictionary_insert(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL) {
        return false;
    }
    
    size_t key_len = strnlen(key, KEY_MAX_LENGTH + 1);
    if (key_len == 0 || key_len > KEY_MAX_LENGTH) {
        return false;
    }
    
    if (dictionary_contains(dict, key)) {
        return false;
    }
    
    unsigned int index = hash(key);
    Entry *new_entry = (Entry *)malloc(sizeof(Entry));
    if (new_entry == NULL) {
        return false;
    }
    
    strncpy(new_entry->key, key, sizeof(new_entry->key) - 1);
    new_entry->key[sizeof(new_entry->key) - 1] = '\0';
    new_entry->value = value;
    new_entry->next = dict->buckets[index];
    dict->buckets[index] = new_entry;
    
    return true;
}

int dictionary_get(Dictionary *dict, const char *key, int *value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }
    
    unsigned int index = hash(key);
    Entry *current = dict->buckets[index];
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            *value = current->value;
            return 0;
        }
        current = current->next;
    }
    
    return -1;
}

void destroy_dictionary(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *current = dict->buckets[i];
        while (current != NULL) {
            Entry *next = current->next;
            free(current);
            current = next;
        }
    }
    
    free(dict);
}

int main(void) {
    Dictionary *dict = create_dictionary();
    if (dict == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return 1;
    }
    
    dictionary_insert(dict, "apple", 1);
    dictionary_insert(dict, "banana", 2);
    dictionary_insert(dict, "cherry", 3);
    dictionary_insert(dict, "date", 4);
    
    const char *keys_to_check[] = {"apple", "banana", "grape", "date", "fig"};
    size_t num_keys = sizeof(keys_to_check) / sizeof(keys_to_check[0]);
    
    for (size_t i = 0; i < num_keys; i++) {
        int value;
        if (dictionary_get(dict, keys_to_check[i], &value) == 0) {
            printf("Key '%s' is present in the dictionary with value %d.\n", keys_to_check[i], value);
        } else {
            printf("Key '%s' is NOT present in the dictionary.\n", keys_to_check[i]);
        }
    }
    
    destroy_dictionary(dict);
    
    return 0;
}