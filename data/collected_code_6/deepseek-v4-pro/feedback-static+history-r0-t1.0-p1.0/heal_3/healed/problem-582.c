#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICT_SIZE 100
#define KEY_SIZE 50

typedef struct {
    char key[KEY_SIZE];
    int value;
    int is_used;
} Entry;

typedef struct {
    Entry entries[DICT_SIZE];
    int count;
} Dictionary;

void dict_init(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
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

int dict_set(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL) {
        return -1;
    }
    if (dict->count >= DICT_SIZE) {
        return -2;
    }

    size_t key_len = strnlen(key, KEY_SIZE);
    if (key_len >= KEY_SIZE) {
        return -3;
    }

    if (key_len + 1 > KEY_SIZE) {
        return -3;
    }

    memcpy(dict->entries[dict->count].key, key, key_len);
    dict->entries[dict->count].key[key_len] = '\0';
    dict->entries[dict->count].value = value;
    dict->entries[dict->count].is_used = 1;
    dict->count++;

    return 0;
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

    {
        const char *test_key = "test";
        size_t test_len = strnlen(test_key, KEY_SIZE);
        if (test_len < KEY_SIZE) {
            if (test_len + 1 <= KEY_SIZE) {
                memcpy(dict.entries[0].key, test_key, test_len);
                dict.entries[0].key[test_len] = '\0';
                dict.entries[0].value = 42;
                dict.entries[0].is_used = 1;
                dict.count = 1;
            }
        }
    }

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