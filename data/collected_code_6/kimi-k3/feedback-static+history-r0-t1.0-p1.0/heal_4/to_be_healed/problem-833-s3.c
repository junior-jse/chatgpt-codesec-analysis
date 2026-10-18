/* Repair history (previous round):
 * - Flawfinder strncpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LENGTH 256
#define MAX_KEYS 100

typedef struct {
    char **keys;
    size_t count;
    size_t capacity;
} KeyList;

int add_key(KeyList *list, const char *key) {
    if (list == NULL || key == NULL) {
        return -1;
    }
    
    size_t key_len = strnlen(key, MAX_KEY_LENGTH);
    if (key_len >= MAX_KEY_LENGTH) {
        return -1;
    }
    
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 8 : list->capacity * 2;
        if (new_capacity > MAX_KEYS) {
            new_capacity = MAX_KEYS;
        }
        if (new_capacity <= list->capacity) {
            return -1;
        }
        char **new_keys = realloc(list->keys, new_capacity * sizeof(char *));
        if (new_keys == NULL) {
            return -1;
        }
        list->keys = new_keys;
        list->capacity = new_capacity;
    }
    
    list->keys[list->count] = malloc(key_len + 1);
    if (list->keys[list->count] == NULL) {
        return -1;
    }
    
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(list->keys[list->count], key, key_len + 1);
    list->count++;
    return 0;
}

char **get_dictionary_keys(const KeyList *list, size_t *num_keys) {
    if (list == NULL || num_keys == NULL) {
        return NULL;
    }
    if (list->count == 0) {
        *num_keys = 0;
        return NULL;
    }
    
    char **keys = malloc(list->count * sizeof(char *));
    if (keys == NULL) {
        return NULL;
    }
    
    for (size_t i = 0; i < list->count; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t key_len = strlen(list->keys[i]) + 1;
        keys[i] = malloc(key_len);
        if (keys[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(keys[j]);
            }
            free(keys);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(keys[i], list->keys[i], key_len);
    }
    *num_keys = list->count;
    return keys;
}

void free_keys(char **keys, size_t num_keys) {
    if (keys == NULL) {
        return;
    }
    for (size_t i = 0; i < num_keys; i++) {
        free(keys[i]);
    }
    free(keys);
}

void free_key_list(KeyList *list) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < list->count; i++) {
        free(list->keys[i]);
    }
    free(list->keys);
    list->keys = NULL;
    list->count = 0;
    list->capacity = 0;
}

int main(void) {
    KeyList dict = {0};
    if (add_key(&dict, "name") != 0) {
        fprintf(stderr, "Failed to add key\n");
        free_key_list(&dict);
        return EXIT_FAILURE;
    }
    if (add_key(&dict, "age") != 0) {
        fprintf(stderr, "Failed to add key\n");
        free_key_list(&dict);
        return EXIT_FAILURE;
    }
    if (add_key(&dict, "city") != 0) {
        fprintf(stderr, "Failed to add key\n");
        free_key_list(&dict);
        return EXIT_FAILURE;
    }
    size_t num_keys = 0;
    char **keys = get_dictionary_keys(&dict, &num_keys);
    if (keys == NULL && num_keys > 0) {
        fprintf(stderr, "Failed to retrieve keys\n");
        free_key_list(&dict);
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < num_keys; i++) {
        printf("%s\n", keys[i]);
    }
    free_keys(keys, num_keys);
    free_key_list(&dict);
    return EXIT_SUCCESS;
}