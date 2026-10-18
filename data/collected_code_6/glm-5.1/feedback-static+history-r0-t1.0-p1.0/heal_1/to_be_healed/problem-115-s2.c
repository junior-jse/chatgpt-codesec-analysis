#include <stdio.h>
#include <stdlib.h>

typedef struct {
    /* Possible weaknesses found:
     *  struct member 'Dictionary::keys' is never used. [unusedStructMember]
     */
    int *keys;
    /* Possible weaknesses found:
     *  struct member 'Dictionary::values' is never used. [unusedStructMember]
     */
    int *values;
    size_t size;
    /* Possible weaknesses found:
     *  struct member 'Dictionary::capacity' is never used. [unusedStructMember]
     */
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
    Dictionary dict1 = {NULL, NULL, 0, 0};
    Dictionary dict2 = {NULL, NULL, 0, 0};
    
    int keys[] = {1};
    int values[] = {10};
    Dictionary dict3 = {keys, values, 1, 1};

    DictionaryList list1 = {&dict1, 1};
    DictionaryList list2 = {(Dictionary[]){dict1, dict2}, 2};
    DictionaryList list3 = {(Dictionary[]){dict1, dict3}, 2};
    /* Possible weaknesses found:
     *  Variable 'list_null' can be declared as pointer to const [constVariablePointer]
     */
    DictionaryList *list_null = NULL;

    printf("%d\n", are_all_dictionaries_empty(&list1));
    printf("%d\n", are_all_dictionaries_empty(&list2));
    printf("%d\n", are_all_dictionaries_empty(&list3));
    printf("%d\n", are_all_dictionaries_empty(list_null));

    return 0;
}