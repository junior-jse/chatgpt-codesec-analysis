#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int key;
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node **buckets;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary *dictionary_create(size_t capacity) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    dict->buckets = calloc(capacity, sizeof(Node *));
    if (dict->buckets == NULL) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

void dictionary_destroy(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    for (size_t i = 0; i < dict->capacity; i++) {
        Node *current = dict->buckets[i];
        while (current != NULL) {
            Node *next = current->next;
            free(current);
            current = next;
        }
    }
    free(dict->buckets);
    free(dict);
}

int dictionary_is_empty(const Dictionary *dict) {
    if (dict == NULL) {
        return 1;
    }
    return dict->size == 0;
}

int main(void) {
    Dictionary *dict = dictionary_create(16);
    if (dict == NULL) {
        return EXIT_FAILURE;
    }
    
    if (dictionary_is_empty(dict)) {
        printf("Dictionary is empty\n");
    } else {
        printf("Dictionary is not empty\n");
    }

    dictionary_destroy(dict);

    if (dictionary_is_empty(NULL)) {
        printf("NULL dictionary is considered empty\n");
    }

    return EXIT_SUCCESS;
}