#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICT_SIZE 100

typedef struct {
    char key[50];
    int value;
    int is_used;
} Entry;

typedef struct {
    Entry entries[DICT_SIZE];
    int count;
} Dictionary;

void dict_init(Dictionary *dict) {
    dict->count = 0;
    for (int i = 0; i < DICT_SIZE; i++) {
        dict->entries[i].is_used = 0;
        dict->entries[i].value = 0;
        dict->entries[i].key[0] = '\0';
    }
}

int dict_is_empty(const Dictionary *dict) {
    if (dict == NULL) {
        return -1;
    }
    return (dict->count == 0) ? 1 : 0;
}

int main(void) {
    Dictionary dict;
    int result;

    dict_init(&dict);

    result = dict_is_empty(&dict);
    if (result == 1) {
        printf("Dictionary is empty\n");
    } else if (result == 0) {
        printf("Dictionary is not empty\n");
    } else {
        printf("Invalid dictionary pointer\n");
    }

    strcpy(dict.entries[0].key, "test");
    dict.entries[0].value = 42;
    dict.entries[0].is_used = 1;
    dict.count = 1;

    result = dict_is_empty(&dict);
    if (result == 1) {
        printf("Dictionary is empty\n");
    } else if (result == 0) {
        printf("Dictionary is not empty\n");
    } else {
        printf("Invalid dictionary pointer\n");
    }

    return 0;
}