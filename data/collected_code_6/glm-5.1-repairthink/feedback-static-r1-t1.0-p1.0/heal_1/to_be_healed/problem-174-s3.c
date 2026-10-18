#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEYS 128
#define MAX_VALUES 256
#define MAX_STR_LEN 64

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char key[MAX_STR_LEN];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char *values[MAX_VALUES];
    int count;
} DictEntry;

typedef struct {
    DictEntry entries[MAX_KEYS];
    int count;
} Dictionary;

int dict_add(Dictionary *dict, const char *key, const char *value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }

    DictEntry *entry = NULL;
    for (int i = 0; i < dict->count; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            entry = &dict->entries[i];
            break;
        }
    }

    if (entry == NULL) {
        if (dict->count >= MAX_KEYS) {
            return -1;
        }
        entry = &dict->entries[dict->count];
        /* Possible weaknesses found:
         * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
         */
        strncpy(entry->key, key, MAX_STR_LEN - 1);
        entry->key[MAX_STR_LEN - 1] = '\0';
        entry->count = 0;
        dict->count++;
    }

    if (entry->count >= MAX_VALUES) {
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    char *val_copy = (char *)malloc(strlen(value) + 1);
    if (val_copy == NULL) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(val_copy, value);

    entry->values[entry->count] = val_copy;
    entry->count++;

    return 0;
}

void dict_free(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }
    for (int i = 0; i < dict->count; i++) {
        for (int j = 0; j < dict->entries[i].count; j++) {
            free(dict->entries[i].values[j]);
        }
    }
    dict->count = 0;
}

int main(void) {
    Dictionary dict;
    memset(&dict, 0, sizeof(Dictionary));

    const char *keys[] = {"fruit", "color", "fruit", "color", "fruit"};
    const char *values[] = {"apple", "red", "banana", "green", "cherry"};
    size_t seq_len = sizeof(keys) / sizeof(keys[0]);

    for (size_t i = 0; i < seq_len; i++) {
        if (dict_add(&dict, keys[i], values[i]) != 0) {
            dict_free(&dict);
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < dict.count; i++) {
        printf("%s: [", dict.entries[i].key);
        for (int j = 0; j < dict.entries[i].count; j++) {
            printf("%s", dict.entries[i].values[j]);
            if (j < dict.entries[i].count - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }

    dict_free(&dict);

    return EXIT_SUCCESS;
}