#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICTIONARY_SIZE 100

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char key[50];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char value[50];
} Entry;

typedef struct {
    Entry entries[DICTIONARY_SIZE];
    int count;
} Dictionary;

void initDictionary(Dictionary *dict) {
    dict->count = 0;
}

/* Possible weaknesses found:
 *  Parameter 'dict' can be declared as pointer to const [constParameterPointer]
 */
int isEmpty(Dictionary *dict) {
    return dict->count == 0;
}

int insert(Dictionary *dict, const char *key, const char *value) {
    if (dict->count >= DICTIONARY_SIZE) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(dict->entries[dict->count].key, key, sizeof(dict->entries[dict->count].key) - 1);
    dict->entries[dict->count].key[sizeof(dict->entries[dict->count].key) - 1] = '\0';
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(dict->entries[dict->count].value, value, sizeof(dict->entries[dict->count].value) - 1);
    dict->entries[dict->count].value[sizeof(dict->entries[dict->count].value) - 1] = '\0';
    dict->count++;
    return 0;
}

int main() {
    Dictionary dict;
    initDictionary(&dict);

    if (isEmpty(&dict)) {
        printf("Dictionary is empty.\n");
    } else {
        printf("Dictionary is not empty.\n");
    }

    if (insert(&dict, "name", "John") != 0) {
        fprintf(stderr, "Failed to insert entry.\n");
        return EXIT_FAILURE;
    }

    if (isEmpty(&dict)) {
        printf("Dictionary is empty.\n");
    } else {
        printf("Dictionary is not empty.\n");
    }

    return EXIT_SUCCESS;
}