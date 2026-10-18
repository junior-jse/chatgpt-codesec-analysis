#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *keys;
    int *values;
    int size;
    int capacity;
} Dictionary;

typedef struct {
    Dictionary *dictionaries;
    int size;
    int capacity;
} DictionaryList;

Dictionary *create_dictionary(int capacity) {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    dict->keys = (int *)malloc(sizeof(int) * capacity);
    dict->values = (int *)malloc(sizeof(int) * capacity);
    if (dict->keys == NULL || dict->values == NULL) {
        free(dict->keys);
        free(dict->values);
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

void destroy_dictionary(Dictionary *dict) {
    if (dict != NULL) {
        free(dict->keys);
        free(dict->values);
        free(dict);
    }
}

DictionaryList *create_dictionary_list(int capacity) {
    DictionaryList *list = (DictionaryList *)malloc(sizeof(DictionaryList));
    if (list == NULL) {
        return NULL;
    }
    list->dictionaries = (Dictionary *)malloc(sizeof(Dictionary) * capacity);
    if (list->dictionaries == NULL) {
        free(list);
        return NULL;
    }
    list->size = 0;
    list->capacity = capacity;
    return list;
}

void destroy_dictionary_list(DictionaryList *list) {
    if (list != NULL) {
        free(list->dictionaries);
        free(list);
    }
}

bool are_all_dictionaries_empty(const DictionaryList *list) {
    if (list == NULL) {
        return true;
    }
    for (int i = 0; i < list->size; i++) {
        if (list->dictionaries[i].size > 0) {
            return false;
        }
    }
    return true;
}

int main(void) {
    DictionaryList *list = create_dictionary_list(3);
    if (list == NULL) {
        return EXIT_FAILURE;
    }

    Dictionary *dict1 = create_dictionary(2);
    Dictionary *dict2 = create_dictionary(2);
    Dictionary *dict3 = create_dictionary(2);
    if (dict1 == NULL || dict2 == NULL || dict3 == NULL) {
        destroy_dictionary(dict1);
        destroy_dictionary(dict2);
        destroy_dictionary(dict3);
        destroy_dictionary_list(list);
        return EXIT_FAILURE;
    }

    list->dictionaries[0] = *dict1;
    list->dictionaries[1] = *dict2;
    list->dictionaries[2] = *dict3;
    list->size = 3;

    free(dict1);
    free(dict2);
    free(dict3);

    if (are_all_dictionaries_empty(list)) {
        printf("All dictionaries are empty.\n");
    } else {
        printf("Not all dictionaries are empty.\n");
    }

    destroy_dictionary_list(list);
    return EXIT_SUCCESS;
}