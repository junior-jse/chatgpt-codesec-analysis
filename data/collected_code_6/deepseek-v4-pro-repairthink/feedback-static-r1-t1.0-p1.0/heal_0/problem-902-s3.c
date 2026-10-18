#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR 0.75

typedef struct Entry {
    char *key;
    int value;
    int occupied;
    struct Entry *next;
} Entry;

typedef struct Dictionary {
    Entry *buckets;
    int capacity;
    int size;
} Dictionary;

static unsigned long hash_string(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

static char *duplicate_string(const char *str) {
    size_t len = strlen(str);
    char *copy = (char *)malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, str, len + 1);
    return copy;
}

Dictionary *create_dictionary(void) {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    dict->buckets = (Entry *)calloc(INITIAL_CAPACITY, sizeof(Entry));
    if (dict->buckets == NULL) {
        free(dict);
        return NULL;
    }
    dict->capacity = INITIAL_CAPACITY;
    dict->size = 0;
    return dict;
}

static int resize_dictionary(Dictionary *dict, int new_capacity) {
    Entry *new_buckets = (Entry *)calloc(new_capacity, sizeof(Entry));
    if (new_buckets == NULL) {
        return 0;
    }

    for (int i = 0; i < dict->capacity; i++) {
        Entry *entry = &dict->buckets[i];
        if (entry->occupied) {
            unsigned long h = hash_string(entry->key);
            int index = h % new_capacity;
            while (new_buckets[index].occupied) {
                index = (index + 1) % new_capacity;
            }
            new_buckets[index].key = entry->key;
            new_buckets[index].value = entry->value;
            new_buckets[index].occupied = 1;
            new_buckets[index].next = NULL;
        }
        Entry *chain = entry->next;
        while (chain != NULL) {
            Entry *temp = chain;
            chain = chain->next;
            unsigned long h = hash_string(temp->key);
            int index = h % new_capacity;
            while (new_buckets[index].occupied) {
                index = (index + 1) % new_capacity;
            }
            new_buckets[index].key = temp->key;
            new_buckets[index].value = temp->value;
            new_buckets[index].occupied = 1;
            new_buckets[index].next = NULL;
            free(temp);
        }
    }

    free(dict->buckets);
    dict->buckets = new_buckets;
    dict->capacity = new_capacity;
    return 1;
}

int dictionary_set(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL) {
        return 0;
    }

    if ((float)(dict->size + 1) / dict->capacity > LOAD_FACTOR) {
        if (!resize_dictionary(dict, dict->capacity * 2)) {
            return 0;
        }
    }

    unsigned long h = hash_string(key);
    int index = h % dict->capacity;

    while (dict->buckets[index].occupied) {
        if (dict->buckets[index].key != NULL && strcmp(dict->buckets[index].key, key) == 0) {
            dict->buckets[index].value = value;
            return 1;
        }
        if (dict->buckets[index].next == NULL) {
            Entry *new_entry = (Entry *)malloc(sizeof(Entry));
            if (new_entry == NULL) {
                return 0;
            }
            new_entry->key = duplicate_string(key);
            if (new_entry->key == NULL) {
                free(new_entry);
                return 0;
            }
            new_entry->value = value;
            new_entry->occupied = 1;
            new_entry->next = NULL;
            dict->buckets[index].next = new_entry;
            dict->size++;
            return 1;
        }
        Entry *prev = &dict->buckets[index];
        Entry *curr = dict->buckets[index].next;
        while (curr != NULL) {
            if (strcmp(curr->key, key) == 0) {
                curr->value = value;
                return 1;
            }
            prev = curr;
            curr = curr->next;
        }
        Entry *new_entry = (Entry *)malloc(sizeof(Entry));
        if (new_entry == NULL) {
            return 0;
        }
        new_entry->key = duplicate_string(key);
        if (new_entry->key == NULL) {
            free(new_entry);
            return 0;
        }
        new_entry->value = value;
        new_entry->occupied = 1;
        new_entry->next = NULL;
        prev->next = new_entry;
        dict->size++;
        return 1;
    }

    dict->buckets[index].key = duplicate_string(key);
    if (dict->buckets[index].key == NULL) {
        return 0;
    }
    dict->buckets[index].value = value;
    dict->buckets[index].occupied = 1;
    dict->buckets[index].next = NULL;
    dict->size++;
    return 1;
}

int dictionary_get(Dictionary *dict, const char *key, int *value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return 0;
    }

    unsigned long h = hash_string(key);
    int index = h % dict->capacity;

    if (dict->buckets[index].occupied) {
        if (dict->buckets[index].key != NULL && strcmp(dict->buckets[index].key, key) == 0) {
            *value = dict->buckets[index].value;
            return 1;
        }
        Entry *curr = dict->buckets[index].next;
        while (curr != NULL) {
            if (strcmp(curr->key, key) == 0) {
                *value = curr->value;
                return 1;
            }
            curr = curr->next;
        }
    }

    return 0;
}

Dictionary *combine_dictionaries(Dictionary *a, Dictionary *b) {
    if (a == NULL || b == NULL) {
        return NULL;
    }

    Dictionary *result = create_dictionary();
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < a->capacity; i++) {
        Entry *entry = &a->buckets[i];
        if (entry->occupied) {
            if (!dictionary_set(result, entry->key, entry->value)) {
                free_dictionary(result);
                return NULL;
            }
        }
        Entry *chain = entry->next;
        while (chain != NULL) {
            if (!dictionary_set(result, chain->key, chain->value)) {
                free_dictionary(result);
                return NULL;
            }
            chain = chain->next;
        }
    }

    for (int i = 0; i < b->capacity; i++) {
        Entry *entry = &b->buckets[i];
        if (entry->occupied) {
            int existing_value;
            if (dictionary_get(result, entry->key, &existing_value)) {
                if (!dictionary_set(result, entry->key, existing_value + entry->value)) {
                    free_dictionary(result);
                    return NULL;
                }
            } else {
                if (!dictionary_set(result, entry->key, entry->value)) {
                    free_dictionary(result);
                    return NULL;
                }
            }
        }
        Entry *chain = entry->next;
        while (chain != NULL) {
            int existing_value;
            if (dictionary_get(result, chain->key, &existing_value)) {
                if (!dictionary_set(result, chain->key, existing_value + chain->value)) {
                    free_dictionary(result);
                    return NULL;
                }
            } else {
                if (!dictionary_set(result, chain->key, chain->value)) {
                    free_dictionary(result);
                    return NULL;
                }
            }
            chain = chain->next;
        }
    }

    return result;
}

void free_dictionary(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }

    for (int i = 0; i < dict->capacity; i++) {
        if (dict->buckets[i].occupied) {
            free(dict->buckets[i].key);
            dict->buckets[i].key = NULL;
        }
        Entry *curr = dict->buckets[i].next;
        while (curr != NULL) {
            Entry *temp = curr;
            curr = curr->next;
            free(temp->key);
            free(temp);
        }
        dict->buckets[i].next = NULL;
    }

    free(dict->buckets);
    free(dict);
}

int main(void) {
    Dictionary *dict1 = create_dictionary();
    Dictionary *dict2 = create_dictionary();
    Dictionary *combined;
    int value;

    if (dict1 == NULL || dict2 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_dictionary(dict1);
        free_dictionary(dict2);
        return EXIT_FAILURE;
    }

    if (!dictionary_set(dict1, "apple", 5)) {
        fprintf(stderr, "Failed to set key\n");
        free_dictionary(dict1);
        free_dictionary(dict2);
        return EXIT_FAILURE;
    }
    if (!dictionary_set(dict1, "banana", 3)) {
        fprintf(stderr, "Failed to set key\n");
        free_dictionary(dict1);
        free_dictionary(dict2);
        return EXIT_FAILURE;
    }
    if (!dictionary_set(dict1, "cherry", 2)) {
        fprintf(stderr, "Failed to set key\n");
        free_dictionary(dict1);
        free_dictionary(dict2);
        return EXIT_FAILURE;
    }

    if (!dictionary_set(dict2, "banana", 4)) {
        fprintf(stderr, "Failed to set key\n");
        free_dictionary(dict1);
        free_dictionary(dict2);
        return EXIT_FAILURE;
    }
    if (!dictionary_set(dict2, "cherry", 7)) {
        fprintf(stderr, "Failed to set key\n");
        free_dictionary(dict1);
        free_dictionary(dict2);
        return EXIT_FAILURE;
    }
    if (!dictionary_set(dict2, "date", 1)) {
        fprintf(stderr, "Failed to set key\n");
        free_dictionary(dict1);
        free_dictionary(dict2);
        return EXIT_FAILURE;
    }

    combined = combine_dictionaries(dict1, dict2);
    if (combined == NULL) {
        fprintf(stderr, "Failed to combine dictionaries\n");
        free_dictionary(dict1);
        free_dictionary(dict2);
        return EXIT_FAILURE;
    }

    if (dictionary_get(combined, "apple", &value)) {
        printf("apple: %d\n", value);
    }
    if (dictionary_get(combined, "banana", &value)) {
        printf("banana: %d\n", value);
    }
    if (dictionary_get(combined, "cherry", &value)) {
        printf("cherry: %d\n", value);
    }
    if (dictionary_get(combined, "date", &value)) {
        printf("date: %d\n", value);
    }

    free_dictionary(dict1);
    free_dictionary(dict2);
    free_dictionary(combined);

    return EXIT_SUCCESS;
}