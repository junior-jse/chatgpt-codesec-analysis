#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICTIONARY_SIZE 100
#define MAX_STRING_LENGTH 1024

typedef struct {
    char *key;
    char *value;
} Entry;

typedef struct {
    Entry entries[DICTIONARY_SIZE];
    int count;
} Dictionary;

static size_t safe_strlen(const char *s, size_t max_len) {
    size_t len = 0;

    if (s == NULL) {
        return 0;
    }

    while (len < max_len && s[len] != '\0') {
        len++;
    }

    return len;
}

void initDictionary(Dictionary *dict) {
    if (dict == NULL) {
        return;
    }

    dict->count = 0;
}

int isEmpty(const Dictionary *dict) {
    if (dict == NULL) {
        return 1;
    }

    return dict->count == 0;
}

int insert(Dictionary *dict, const char *key, const char *value) {
    size_t key_len;
    size_t value_len;
    char *key_copy;
    char *value_copy;
    int written;

    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }

    if (dict->count < 0 || dict->count >= DICTIONARY_SIZE) {
        return -1;
    }

    key_len = safe_strlen(key, MAX_STRING_LENGTH);
    if (key_len >= MAX_STRING_LENGTH) {
        return -1;
    }
    key_len += 1;

    value_len = safe_strlen(value, MAX_STRING_LENGTH);
    if (value_len >= MAX_STRING_LENGTH) {
        return -1;
    }
    value_len += 1;

    key_copy = malloc(key_len);
    if (key_copy == NULL) {
        return -1;
    }

    value_copy = malloc(value_len);
    if (value_copy == NULL) {
        free(key_copy);
        return -1;
    }

    written = snprintf(key_copy, key_len, "%s", key);
    if (written < 0 || (size_t)written >= key_len) {
        free(key_copy);
        free(value_copy);
        return -1;
    }

    written = snprintf(value_copy, value_len, "%s", value);
    if (written < 0 || (size_t)written >= value_len) {
        free(key_copy);
        free(value_copy);
        return -1;
    }

    dict->entries[dict->count].key = key_copy;
    dict->entries[dict->count].value = value_copy;
    dict->count++;

    return 0;
}

void freeDictionary(Dictionary *dict) {
    int i;

    if (dict == NULL) {
        return;
    }

    for (i = 0; i < dict->count; i++) {
        free(dict->entries[i].key);
        free(dict->entries[i].value);
        dict->entries[i].key = NULL;
        dict->entries[i].value = NULL;
    }

    dict->count = 0;
}

int main(void) {
    Dictionary dict;
    initDictionary(&dict);

    if (isEmpty(&dict)) {
        printf("Dictionary is empty.\n");
    } else {
        printf("Dictionary is not empty.\n");
    }

    if (insert(&dict, "name", "John") != 0) {
        fprintf(stderr, "Failed to insert entry.\n");
        freeDictionary(&dict);
        return EXIT_FAILURE;
    }

    if (isEmpty(&dict)) {
        printf("Dictionary is empty.\n");
    } else {
        printf("Dictionary is not empty.\n");
    }

    freeDictionary(&dict);

    return EXIT_SUCCESS;
}