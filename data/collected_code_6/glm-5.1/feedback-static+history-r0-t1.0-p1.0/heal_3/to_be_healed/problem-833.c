#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 128
#define KEY_LENGTH 64

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char keys[MAX_ENTRIES][KEY_LENGTH];
    int values[MAX_ENTRIES];
    int size;
} Dictionary;

char** get_dictionary_keys(Dictionary* dict, int* out_size) {
    if (dict == NULL || out_size == NULL) {
        return NULL;
    }

    if (dict->size <= 0 || dict->size > MAX_ENTRIES) {
        *out_size = 0;
        return NULL;
    }

    char** key_list = malloc(dict->size * sizeof(char*));
    if (key_list == NULL) {
        *out_size = 0;
        return NULL;
    }

    for (int i = 0; i < dict->size; i++) {
        key_list[i] = malloc(KEY_LENGTH * sizeof(char));
        if (key_list[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(key_list[j]);
            }
            free(key_list);
            *out_size = 0;
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
         */
        strncpy(key_list[i], dict->keys[i], KEY_LENGTH - 1);
        key_list[i][KEY_LENGTH - 1] = '\0';
    }

    *out_size = dict->size;
    return key_list;
}

void free_key_list(char** key_list, int size) {
    if (key_list == NULL) {
        return;
    }
    for (int i = 0; i < size; i++) {
        free(key_list[i]);
    }
    free(key_list);
}

int main() {
    Dictionary dict;
    dict.size = 3;
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). Risk is low because the source is a constant string. (risk 1, buffer)
     */
    strncpy(dict.keys[0], "apple", KEY_LENGTH - 1);
    dict.keys[0][KEY_LENGTH - 1] = '\0';
    dict.values[0] = 1;
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). Risk is low because the source is a constant string. (risk 1, buffer)
     */
    strncpy(dict.keys[1], "banana", KEY_LENGTH - 1);
    dict.keys[1][KEY_LENGTH - 1] = '\0';
    dict.values[1] = 2;
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). Risk is low because the source is a constant string. (risk 1, buffer)
     */
    strncpy(dict.keys[2], "cherry", KEY_LENGTH - 1);
    dict.keys[2][KEY_LENGTH - 1] = '\0';
    dict.values[2] = 3;

    int list_size = 0;
    char** keys = get_dictionary_keys(&dict, &list_size);

    if (keys != NULL) {
        for (int i = 0; i < list_size; i++) {
            printf("%s\n", keys[i]);
        }
        free_key_list(keys, list_size);
    }

    return 0;
}