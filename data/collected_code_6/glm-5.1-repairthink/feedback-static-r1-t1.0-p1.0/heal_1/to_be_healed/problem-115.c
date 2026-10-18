#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    Dictionary *items;
    size_t size;
} DictionaryList;

bool are_all_dictionaries_empty(const DictionaryList *list) {
    if (list == NULL) {
        return true;
    }

    for (size_t i = 0; i < list->size; i++) {
        if (list->items[i].size > 0) {
            return false;
        }
    }

    return true;
}

int main(void) {
    Dictionary d1 = {NULL, NULL, 0, 0};
    Dictionary d2 = {NULL, NULL, 0, 0};
    
    int keys[] = {1};
    int values[] = {10};
    Dictionary d3 = {keys, values, 1, 1};

    DictionaryList list1 = {&d1, 1};
    DictionaryList list2 = {(Dictionary[]){d1, d2}, 2};
    DictionaryList list3 = {(Dictionary[]){d1, d3}, 2};
    DictionaryList list4 = {NULL, 0};

    printf("%d\n", are_all_dictionaries_empty(&list1));
    printf("%d\n", are_all_dictionaries_empty(&list2));
    printf("%d\n", are_all_dictionaries_empty(&list3));
    printf("%d\n", are_all_dictionaries_empty(&list4));
    printf("%d\n", are_all_dictionaries_empty(NULL));

    return 0;
}