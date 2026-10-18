#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *keys;
    int *values;
    size_t size;
    size_t capacity;
} Dictionary;

typedef struct {
    Dictionary *dictionaries;
    size_t size;
} DictionaryList;

int are_all_dictionaries_empty(const DictionaryList *list) {
    if (list == NULL) {
        return 1;
    }

    for (size_t i = 0; i < list->size; i++) {
        if (list->dictionaries[i].size > 0) {
            return 0;
        }
    }

    return 1;
}

int get_value(const Dictionary *dict, int key) {
    if (dict == NULL || dict->keys == NULL || dict->values == NULL) {
        return -1;
    }

    for (size_t i = 0; i < dict->size && i < dict->capacity; i++) {
        if (dict->keys[i] == key) {
            return dict->values[i];
        }
    }

    return -1;
}

int main(void) {
    Dictionary dict1 = {NULL, NULL, 0, 0};
    Dictionary dict2 = {NULL, NULL, 0, 0};
    
    int keys[] = {1};
    int values[] = {10};
    Dictionary dict3 = {keys, values, 1, 1};

    DictionaryList list1 = {&dict1, 1};
    DictionaryList list2 = {(Dictionary[]){dict1, dict2}, 2};
    DictionaryList list3 = {(Dictionary[]){dict1, dict3}, 2};
    const DictionaryList *list_null = NULL;

    printf("%d\n", are_all_dictionaries_empty(&list1));
    printf("%d\n", are_all_dictionaries_empty(&list2));
    printf("%d\n", are_all_dictionaries_empty(&list3));
    printf("%d\n", are_all_dictionaries_empty(list_null));
    printf("%d\n", get_value(&dict3, 1));

    return 0;
}