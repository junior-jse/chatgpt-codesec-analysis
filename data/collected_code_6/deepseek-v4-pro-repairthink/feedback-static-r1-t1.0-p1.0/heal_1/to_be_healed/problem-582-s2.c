#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct KeyValue {
    char *key;
    int value;
    struct KeyValue *next;
} KeyValue;

typedef struct {
    KeyValue **buckets;
    size_t size;
    size_t count;
} Dictionary;

Dictionary *create_dictionary(size_t size) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    if (!dict) return NULL;
    dict->buckets = calloc(size, sizeof(KeyValue *));
    if (!dict->buckets) {
        free(dict);
        return NULL;
    }
    dict->size = size;
    dict->count = 0;
    return dict;
}

void destroy_dictionary(Dictionary *dict) {
    if (!dict) return;
    for (size_t i = 0; i < dict->size; i++) {
        KeyValue *current = dict->buckets[i];
        while (current) {
            KeyValue *next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    free(dict->buckets);
    free(dict);
}

bool dictionary_is_empty(const Dictionary *dict) {
    if (!dict) return true;
    return dict->count == 0;
}

int main(void) {
    Dictionary *dict = create_dictionary(16);
    if (!dict) {
        fprintf(stderr, "Failed to create dictionary\n");
        return 1;
    }

    if (dictionary_is_empty(dict)) {
        printf("Dictionary is empty\n");
    } else {
        printf("Dictionary is not empty\n");
    }

    /* Possible weaknesses found:
     *  Calling function 'dictionary_is_empty' returns 1
     *  Condition 'dictionary_is_empty(NULL)' is always true
     *  Condition 'dictionary_is_empty(NULL)' is always true [knownConditionTrueFalse]
     */
    if (dictionary_is_empty(NULL)) {
        printf("NULL dictionary considered empty\n");
    }

    destroy_dictionary(dict);
    return 0;
}