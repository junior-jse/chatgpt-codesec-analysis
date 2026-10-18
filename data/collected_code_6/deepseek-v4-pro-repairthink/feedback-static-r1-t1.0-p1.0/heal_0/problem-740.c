#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} KeyValuePair;

typedef struct {
    KeyValuePair *pairs;
    size_t count;
} Dictionary;

Dictionary *tuple_to_dict(char **tuple, size_t tuple_size) {
    Dictionary *dict;
    size_t i;

    if (tuple == NULL || tuple_size == 0 || tuple_size % 2 != 0) {
        return NULL;
    }

    dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }

    dict->count = tuple_size / 2;
    dict->pairs = (KeyValuePair *)calloc(dict->count, sizeof(KeyValuePair));
    if (dict->pairs == NULL) {
        free(dict);
        return NULL;
    }

    for (i = 0; i < dict->count; i++) {
        dict->pairs[i].key = tuple[2 * i];
        dict->pairs[i].value = tuple[2 * i + 1];
    }

    return dict;
}

void free_dictionary(Dictionary *dict) {
    if (dict != NULL) {
        free(dict->pairs);
        free(dict);
    }
}

int main(void) {
    char *tuple[] = {"name", "Alice", "age", "30", "city", "New York"};
    size_t tuple_size = sizeof(tuple) / sizeof(tuple[0]);
    Dictionary *dict;
    size_t i;

    dict = tuple_to_dict(tuple, tuple_size);
    if (dict == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return 1;
    }

    for (i = 0; i < dict->count; i++) {
        printf("%s: %s\n", dict->pairs[i].key, dict->pairs[i].value);
    }

    free_dictionary(dict);
    return 0;
}