#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#define INITIAL_CAPACITY 8
#define LOAD_FACTOR 0.75f

typedef struct KeyValue {
    char *key;
    void *value;
    struct KeyValue *next;
} KeyValue;

typedef struct {
    KeyValue **buckets;
    size_t capacity;
    size_t size;
} Dictionary;

static unsigned long hash_string(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++) != '\0') {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

static Dictionary *dict_create(void) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    dict->capacity = INITIAL_CAPACITY;
    dict->size = 0;
    dict->buckets = calloc(dict->capacity, sizeof(KeyValue *));
    if (dict->buckets == NULL) {
        free(dict);
        return NULL;
    }
    return dict;
}

static int dict_resize(Dictionary *dict, size_t new_capacity) {
    KeyValue **new_buckets = calloc(new_capacity, sizeof(KeyValue *));
    if (new_buckets == NULL) {
        return -1;
    }
    
    for (size_t i = 0; i < dict->capacity; i++) {
        KeyValue *entry = dict->buckets[i];
        while (entry != NULL) {
            KeyValue *next = entry->next;
            size_t index = hash_string(entry->key) % new_capacity;
            entry->next = new_buckets[index];
            new_buckets[index] = entry;
            entry = next;
        }
    }
    
    free(dict->buckets);
    dict->buckets = new_buckets;
    dict->capacity = new_capacity;
    return 0;
}

static int dict_put(Dictionary *dict, const char *key, void *value) {
    if (dict == NULL || key == NULL) {
        return -1;
    }

    if (dict->size >= (size_t)(dict->capacity * LOAD_FACTOR)) {
        size_t new_capacity = dict->capacity * 2;
        if (new_capacity < dict->capacity) {
            return -1;
        }
        if (dict_resize(dict, new_capacity) != 0) {
            return -1;
        }
    }
    
    size_t index = hash_string(key) % dict->capacity;
    KeyValue *entry = dict->buckets[index];
    
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return 0;
        }
        entry = entry->next;
    }
    
    size_t key_len = strnlen(key, SIZE_MAX);
    if (key_len == SIZE_MAX) {
        return -1;
    }
    
    KeyValue *new_entry = malloc(sizeof(KeyValue));
    if (new_entry == NULL) {
        return -1;
    }
    
    new_entry->key = malloc(key_len + 1);
    if (new_entry->key == NULL) {
        free(new_entry);
        return -1;
    }
    
    memcpy(new_entry->key, key, key_len);
    new_entry->key[key_len] = '\0';
    new_entry->value = value;
    new_entry->next = dict->buckets[index];
    dict->buckets[index] = new_entry;
    dict->size++;
    return 0;
}

static void *dict_get(const Dictionary *dict, const char *key) {
    if (dict == NULL || key == NULL) {
        return NULL;
    }
    
    size_t index = hash_string(key) % dict->capacity;
    const KeyValue *entry = dict->buckets[index];
    
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    
    return NULL;
}

static void dict_destroy(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    
    for (size_t i = 0; i < dict->capacity; i++) {
        KeyValue *entry = dict->buckets[i];
        while (entry != NULL) {
            KeyValue *next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    
    free(dict->buckets);
    free(dict);
}

char **dict_get_keys(const Dictionary *dict) {
    if (dict == NULL || dict->size == 0) {
        return NULL;
    }
    
    char **keys = malloc(sizeof(char *) * (dict->size + 1));
    if (keys == NULL) {
        return NULL;
    }
    
    size_t index = 0;
    for (size_t i = 0; i < dict->capacity; i++) {
        const KeyValue *entry = dict->buckets[i];
        while (entry != NULL) {
            size_t key_len = strnlen(entry->key, SIZE_MAX);
            if (key_len == SIZE_MAX) {
                for (size_t j = 0; j < index; j++) {
                    free(keys[j]);
                }
                free(keys);
                return NULL;
            }
            
            keys[index] = malloc(key_len + 1);
            if (keys[index] == NULL) {
                for (size_t j = 0; j < index; j++) {
                    free(keys[j]);
                }
                free(keys);
                return NULL;
            }
            
            memcpy(keys[index], entry->key, key_len);
            keys[index][key_len] = '\0';
            index++;
            entry = entry->next;
        }
    }
    
    keys[index] = NULL;
    return keys;
}

int main(void) {
    Dictionary *dict = dict_create();
    if (dict == NULL) {
        return 1;
    }
    
    int value1 = 10;
    int value2 = 20;
    int value3 = 30;
    
    if (dict_put(dict, "one", &value1) != 0) {
        dict_destroy(dict);
        return 1;
    }
    if (dict_put(dict, "two", &value2) != 0) {
        dict_destroy(dict);
        return 1;
    }
    if (dict_put(dict, "three", &value3) != 0) {
        dict_destroy(dict);
        return 1;
    }
    
    const int *val = (const int *)dict_get(dict, "two");
    if (val != NULL) {
        printf("Value for 'two': %d\n", *val);
    }
    
    char **keys = dict_get_keys(dict);
    if (keys != NULL) {
        for (size_t i = 0; keys[i] != NULL; i++) {
            printf("%s\n", keys[i]);
            free(keys[i]);
        }
        free(keys);
    }
    
    dict_destroy(dict);
    return 0;
}