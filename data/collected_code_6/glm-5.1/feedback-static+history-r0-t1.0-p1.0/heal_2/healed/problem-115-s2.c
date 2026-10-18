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

int main(void) {
    int keys1[] = {1};
    int values1[] = {10};

    Dictionary dict1;
    dict1.keys = NULL;
    dict1.values = NULL;
    dict1.size = 0;
    dict1.capacity = 0;

    Dictionary dict2;
    dict2.keys = NULL;
    dict2.values = NULL;
    dict2.size = 0;
    dict2.capacity = 0;

    Dictionary dict3;
    dict3.keys = keys1;
    dict3.values = values1;
    dict3.size = 1;
    dict3.capacity = 1;

    DictionaryList list1;
    list1.dictionaries = &dict1;
    list1.size = 1;

    DictionaryList list2;
    list2.dictionaries = (Dictionary[]){dict1, dict2};
    list2.size = 2;

    DictionaryList list3;
    list3.dictionaries = (Dictionary[]){dict1, dict3};
    list3.size = 2;

    const DictionaryList *list_null = NULL;

    printf("%d\n", are_all_dictionaries_empty(&list1));
    printf("%d\n", are_all_dictionaries_empty(&list2));
    printf("%d\n", are_all_dictionaries_empty(&list3));
    printf("%d\n", are_all_dictionaries_empty(list_null));

    return 0;
}