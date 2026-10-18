#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **keys;
    int *values;
    int size;
    int capacity;
} Dictionary;

Dictionary *dict_create(int capacity) {
    Dictionary *dict = malloc(sizeof(Dictionary));
    dict->keys = malloc(sizeof(char *) * capacity);
    dict->values = malloc(sizeof(int) * capacity);
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

void dict_free(Dictionary *dict) {
    if (dict != NULL) {
        for (int i = 0; i < dict->size; ++i) {
            free(dict->keys[i]);
        }
        free(dict->keys);
        free(dict->values);
        free(dict);
    }
}

int dict_insert(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL || dict->size >= dict->capacity) {
        return -1;
    }
    dict->keys[dict->size] = strdup(key);
    if (dict->keys[dict->size] == NULL) {
        return -1;
    }
    dict->values[dict->size] = value;
    dict->size++;
    return 0;
}

Dictionary *dict_merge(const Dictionary *d1, const Dictionary *d2) {
    if (d1 == NULL || d2 == NULL) {
        return NULL;
    }
    int total_capacity = d1->size + d2->size;
    if (total_capacity == 0) {
        return dict_create(1);
    }
    Dictionary *merged = dict_create(total_capacity);
    if (merged == NULL) {
        return NULL;
    }
    for (int i = 0; i < d1->size; ++i) {
        if (dict_insert(merged, d1->keys[i], d1->values[i]) != 0) {
            dict_free(merged);
            return NULL;
        }
    }
    for (int i = 0; i < d2->size; ++i) {
        int found = 0;
        for (int j = 0; j < d1->size; ++j) {
            if (strcmp(d2->keys[i], d1->keys[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            if (dict_insert(merged, d2->keys[i], d2->values[i]) != 0) {
                dict_free(merged);
                return NULL;
            }
        }
    }
    return merged;
}

int main(void) {
    Dictionary *d1 = dict_create(4);
    if (d1 == NULL) return EXIT_FAILURE;
    Dictionary *d2 = dict_create(4);
    if (d2 == NULL) { dict_free(d1); return EXIT_FAILURE; }

    dict_insert(d1, "a", 1);
    dict_insert(d1, "b", 2);
    dict_insert(d1, "c", 3);

    dict_insert(d2, "b", 20);
    dict_insert(d2, "d", 4);
    dict_insert(d2, "e", 5);

    Dictionary *merged = dict_merge(d1, d2);
    if (merged == NULL) {
        dict_free(d1);
        dict_free(d2);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < merged->size; ++i) {
        printf("%s: %d\n", merged->keys[i], merged->values[i]);
    }

    dict_free(d1);
    dict_free(d2);
    dict_free(merged);

    return EXIT_SUCCESS;
}