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
    size_t count;
} Dictionary;

Dictionary *dictionary_create(size_t size) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) {
        return NULL;
    }
    dict->buckets = calloc(size, sizeof(Node *));
    if (!dict->buckets) {
        free(dict);
        return NULL;
    }
    dict->size = size;
    dict->count = 0;
    return dict;
}

void dictionary_destroy(Dictionary *dict) {
    if (!dict) {
        return;
    }
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

int dictionary_insert(Dictionary *dict, const char *key, int value) {
    if (!dict || !key) {
        return 0;
    }
    size_t index = 0;
    for (size_t i = 0; key[i]; i++) {
        index = (index * 31 + key[i]) % dict->size;
    }
    Node *current = dict->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return 1;
        }
        current = current->next;
    }
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) {
        return 0;
    }
    new_node->key = strdup(key);
    if (!new_node->key) {
        free(new_node);
        return 0;
    }
    new_node->value = value;
    new_node->next = dict->buckets[index];
    dict->buckets[index] = new_node;
    dict->count++;
    return 1;
}

int dictionary_is_empty(const Dictionary *dict) {
    if (!dict) {
        return 1;
    }
    return dict->count == 0;
}

int main(void) {
    Dictionary *dict = dictionary_create(16);
    if (!dict) {
        return 1;
    }

    if (dictionary_is_empty(dict)) {
        printf("Dictionary is empty\n");
    } else {
        printf("Dictionary is not empty\n");
    }

    if (dictionary_insert(dict, "key1", 1)) {
        printf("Inserted key1\n");
    }

    if (dictionary_is_empty(dict)) {
        printf("Dictionary is empty\n");
    } else {
        printf("Dictionary is not empty\n");
    }

    dictionary_destroy(dict);
    dict = NULL;

    return 0;
}