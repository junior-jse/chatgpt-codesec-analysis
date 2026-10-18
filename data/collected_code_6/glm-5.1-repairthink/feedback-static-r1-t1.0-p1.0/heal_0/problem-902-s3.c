#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *key;
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node **buckets;
    size_t size;
} Dictionary;

unsigned long hash_key(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

Dictionary *dict_create(size_t size) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) return NULL;
    dict->size = size;
    dict->buckets = calloc(size, sizeof(Node *));
    if (!dict->buckets) {
        free(dict);
        return NULL;
    }
    return dict;
}

int dict_set(Dictionary *dict, const char *key, int value) {
    if (!dict || !key) return 0;
    unsigned long index = hash_key(key) % dict->size;
    Node *current = dict->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return 1;
        }
        current = current->next;
    }
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) return 0;
    new_node->key = strdup(key);
    if (!new_node->key) {
        free(new_node);
        return 0;
    }
    new_node->value = value;
    new_node->next = dict->buckets[index];
    dict->buckets[index] = new_node;
    return 1;
}

int dict_add(Dictionary *dict, const char *key, int value) {
    if (!dict || !key) return 0;
    unsigned long index = hash_key(key) % dict->size;
    Node *current = dict->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value += value;
            return 1;
        }
        current = current->next;
    }
    return dict_set(dict, key, value);
}

void dict_free(Dictionary *dict) {
    if (!dict) return;
    for (size_t i = 0; i < dict->size; i++) {
        Node *current = dict->buckets[i];
        while (current) {
            Node *next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    free(dict->buckets);
    free(dict);
}

Dictionary *combine_dictionaries(Dictionary *d1, Dictionary *d2) {
    if (!d1 || !d2) return NULL;
    Dictionary *result = dict_create(d1->size + d2->size);
    if (!result) return NULL;
    for (size_t i = 0; i < d1->size; i++) {
        Node *current = d1->buckets[i];
        while (current) {
            if (!dict_add(result, current->key, current->value)) {
                dict_free(result);
                return NULL;
            }
            current = current->next;
        }
    }
    for (size_t i = 0; i < d2->size; i++) {
        Node *current = d2->buckets[i];
        while (current) {
            if (!dict_add(result, current->key, current->value)) {
                dict_free(result);
                return NULL;
            }
            current = current->next;
        }
    }
    return result;
}

void dict_print(Dictionary *dict) {
    if (!dict) return;
    for (size_t i = 0; i < dict->size; i++) {
        Node *current = dict->buckets[i];
        while (current) {
            printf("%s: %d\n", current->key, current->value);
            current = current->next;
        }
    }
}

int main(void) {
    Dictionary *d1 = dict_create(16);
    Dictionary *d2 = dict_create(16);
    if (!d1 || !d2) {
        dict_free(d1);
        dict_free(d2);
        return 1;
    }
    dict_set(d1, "apple", 5);
    dict_set(d1, "banana", 3);
    dict_set(d2, "apple", 2);
    dict_set(d2, "cherry", 7);
    Dictionary *combined = combine_dictionaries(d1, d2);
    if (!combined) {
        dict_free(d1);
        dict_free(d2);
        return 1;
    }
    dict_print(combined);
    dict_free(d1);
    dict_free(d2);
    dict_free(combined);
    return 0;
}