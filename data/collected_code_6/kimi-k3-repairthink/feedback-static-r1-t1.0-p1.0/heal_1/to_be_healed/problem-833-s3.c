#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LENGTH 256
#define MAX_KEYS 100

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char keys[MAX_KEYS][MAX_KEY_LENGTH];
    size_t count;
} KeyList;

int add_key(KeyList *list, const char *key) {
    if (list == NULL || key == NULL) {
        return -1;
    }
    if (list->count >= MAX_KEYS) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t key_len = strlen(key);
    if (key_len >= MAX_KEY_LENGTH) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(list->keys[list->count], key, MAX_KEY_LENGTH - 1);
    list->keys[list->count][MAX_KEY_LENGTH - 1] = '\0';
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
        keys[i] = malloc(MAX_KEY_LENGTH);
        if (keys[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(keys[j]);
            }
            free(keys);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
         */
        strncpy(keys[i], list->keys[i], MAX_KEY_LENGTH - 1);
        keys[i][MAX_KEY_LENGTH - 1] = '\0';
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

int main(void) {
    KeyList dict = {0};
    if (add_key(&dict, "name") != 0) {
        fprintf(stderr, "Failed to add key\n");
        return EXIT_FAILURE;
    }
    if (add_key(&dict, "age") != 0) {
        fprintf(stderr, "Failed to add key\n");
        return EXIT_FAILURE;
    }
    if (add_key(&dict, "city") != 0) {
        fprintf(stderr, "Failed to add key\n");
        return EXIT_FAILURE;
    }
    size_t num_keys = 0;
    char **keys = get_dictionary_keys(&dict, &num_keys);
    if (keys == NULL && num_keys > 0) {
        fprintf(stderr, "Failed to retrieve keys\n");
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < num_keys; i++) {
        printf("%s\n", keys[i]);
    }
    free_keys(keys, num_keys);
    return EXIT_SUCCESS;
}