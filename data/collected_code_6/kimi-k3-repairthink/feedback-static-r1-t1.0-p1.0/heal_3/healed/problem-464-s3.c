#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICT_SIZE 100
#define MAX_KEY_LENGTH 50

typedef struct {
    char *key;
    int value;
} DictEntry;

typedef struct {
    DictEntry entries[MAX_DICT_SIZE];
    size_t size;
} Dictionary;

static size_t safe_strnlen(const char *s, size_t maxlen)
{
    size_t len = 0;

    if (s == NULL) {
        return 0;
    }

    while (len < maxlen && s[len] != '\0') {
        len++;
    }

    return len;
}

static void safe_strcopy(char *dst, const char *src, size_t len)
{
    size_t i;

    for (i = 0; i < len; i++) {
        dst[i] = src[i];
    }

    dst[len] = '\0';
}

int dict_init(Dictionary *dict)
{
    if (dict == NULL) {
        return -1;
    }

    memset(dict, 0, sizeof(*dict));

    return 0;
}

void dict_free(Dictionary *dict)
{
    size_t i;

    if (dict == NULL) {
        return;
    }

    for (i = 0; i < dict->size && i < MAX_DICT_SIZE; i++) {
        free(dict->entries[i].key);
        dict->entries[i].key = NULL;
    }

    dict->size = 0;
}

int dict_add(Dictionary *dict, const char *key, int value)
{
    size_t key_len;
    DictEntry *entry;
    char *key_copy;

    if (dict == NULL || key == NULL) {
        return -1;
    }

    if (dict->size >= MAX_DICT_SIZE) {
        return -1;
    }

    key_len = safe_strnlen(key, MAX_KEY_LENGTH);
    if (key_len == 0 || key_len >= MAX_KEY_LENGTH) {
        return -1;
    }

    key_copy = malloc(key_len + 1);
    if (key_copy == NULL) {
        return -1;
    }

    safe_strcopy(key_copy, key, key_len);

    entry = &dict->entries[dict->size];
    entry->key = key_copy;
    entry->value = value;
    dict->size++;

    return 0;
}

int dict_all_values_same(const Dictionary *dict, int *result)
{
    int first_value;
    size_t i;

    if (dict == NULL || result == NULL) {
        return -1;
    }

    if (dict->size > MAX_DICT_SIZE) {
        return -1;
    }

    if (dict->size == 0) {
        *result = 1;
        return 0;
    }

    first_value = dict->entries[0].value;

    for (i = 1; i < dict->size; i++) {
        if (dict->entries[i].value != first_value) {
            *result = 0;
            return 0;
        }
    }

    *result = 1;
    return 0;
}

int main(void)
{
    Dictionary dict;
    int result;

    if (dict_init(&dict) != 0) {
        fprintf(stderr, "Failed to initialize dictionary\n");
        return EXIT_FAILURE;
    }

    if (dict_add(&dict, "apple", 5) != 0) {
        fprintf(stderr, "Failed to add entry\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    if (dict_add(&dict, "banana", 5) != 0) {
        fprintf(stderr, "Failed to add entry\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    if (dict_add(&dict, "cherry", 5) != 0) {
        fprintf(stderr, "Failed to add entry\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    if (dict_all_values_same(&dict, &result) != 0) {
        fprintf(stderr, "Failed to check dictionary values\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    if (result) {
        printf("All values are the same\n");
    } else {
        printf("Values are different\n");
    }

    if (dict_add(&dict, "date", 10) != 0) {
        fprintf(stderr, "Failed to add entry\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    if (dict_all_values_same(&dict, &result) != 0) {
        fprintf(stderr, "Failed to check dictionary values\n");
        dict_free(&dict);
        return EXIT_FAILURE;
    }

    if (result) {
        printf("All values are the same\n");
    } else {
        printf("Values are different\n");
    }

    dict_free(&dict);

    return EXIT_SUCCESS;
}