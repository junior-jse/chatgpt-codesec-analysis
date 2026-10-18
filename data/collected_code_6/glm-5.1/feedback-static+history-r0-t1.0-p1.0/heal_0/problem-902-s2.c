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

Dictionary *create_dictionary(size_t size) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) return NULL;
    dict->buckets = calloc(size, sizeof(Node *));
    if (!dict->buckets) {
        free(dict);
        return NULL;
    }
    dict->size = size;
    return dict;
}

unsigned long hash(const char *str, size_t size) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % size;
}

int insert_or_add(Dictionary *dict, const char *key, int value) {
    if (!dict || !key) return 0;
    unsigned long index = hash(key, dict->size);
    Node *current = dict->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value += value;
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

void free_dictionary(Dictionary *dict) {
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

Dictionary *combine_dictionaries(Dictionary *dict1, Dictionary *dict2) {
    if (!dict1) return NULL;
    size_t new_size = dict1->size + (dict2 ? dict2->size : 0);
    if (new_size == 0) new_size = 1;
    Dictionary *combined = create_dictionary(new_size);
    if (!combined) return NULL;
    for (size_t i = 0; i < dict1->size; i++) {
        Node *current = dict1->buckets[i];
        while (current) {
            if (!insert_or_add(combined, current->key, current->value)) {
                free_dictionary(combined);
                return NULL;
            }
            current = current->next;
        }
    }
    if (dict2) {
        for (size_t i = 0; i < dict2->size; i++) {
            Node *current = dict2->buckets[i];
            while (current) {
                if (!insert_or_add(combined, current->key, current->value)) {
                    free_dictionary(combined);
                    return NULL;
                }
                current = current->next;
            }
        }
    }
    return combined;
}

void print_dictionary(Dictionary *dict) {
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
    Dictionary *dict1 = create_dictionary(10);
    Dictionary *dict2 = create_dictionary(10);
    if (!dict1 || !dict2) {
        free_dictionary(dict1);
        free_dictionary(dict2);
        return EXIT_FAILURE;
    }

    insert_or_add(dict1, "apple", 5);
    insert_or_add(dict1, "banana", 3);
    insert_or_add(dict2, "apple", 2);
    insert_or_add(dict2, "cherry", 7);

    Dictionary *combined = combine_dictionaries(dict1, dict2);
    if (!combined) {
        free_dictionary(dict1);
        free_dictionary(dict2);
        return EXIT_FAILURE;
    }

    print_dictionary(combined);

    free_dictionary(dict1);
    free_dictionary(dict2);
    free_dictionary(combined);

    return EXIT_SUCCESS;
}