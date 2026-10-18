#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 4096

typedef struct DictNode {
    char *key;
    char *value;
    struct DictNode *next;
} DictNode;

typedef struct {
    DictNode **buckets;
    size_t bucket_count;
    size_t size;
} Dictionary;

static size_t bounded_length(const char *s, size_t max_len)
{
    size_t len = 0;

    if (s == NULL) {
        return 0;
    }

    while (len < max_len && s[len] != '\0') {
        len++;
    }

    return len;
}

static unsigned long hash_key(const char *key, size_t bucket_count)
{
    unsigned long hash = 5381UL;
    size_t len;
    size_t i;

    if (key == NULL || bucket_count == 0) {
        return 0UL;
    }

    len = bounded_length(key, MAX_STRING_LENGTH);

    for (i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + (unsigned long)(unsigned char)key[i];
    }

    return hash % bucket_count;
}

static Dictionary *dict_create(size_t bucket_count)
{
    Dictionary *dict;

    if (bucket_count == 0) {
        return NULL;
    }

    dict = malloc(sizeof(*dict));
    if (dict == NULL) {
        return NULL;
    }

    dict->buckets = calloc(bucket_count, sizeof(*dict->buckets));
    if (dict->buckets == NULL) {
        free(dict);
        return NULL;
    }

    dict->bucket_count = bucket_count;
    dict->size = 0;

    return dict;
}

static char *duplicate_string(const char *s)
{
    size_t len;
    char *copy;

    if (s == NULL) {
        return NULL;
    }

    len = bounded_length(s, MAX_STRING_LENGTH);
    if (len == MAX_STRING_LENGTH) {
        return NULL;
    }

    copy = malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        copy[i] = s[i];
    }
    copy[len] = '\0';

    return copy;
}

static int dict_insert(Dictionary *dict, const char *key, const char *value)
{
    DictNode *node;
    DictNode *current;
    unsigned long index;
    char *key_copy;
    char *value_copy;

    if (dict == NULL || key == NULL || value == NULL ||
        dict->buckets == NULL || dict->bucket_count == 0) {
        return -1;
    }

    if (bounded_length(key, MAX_STRING_LENGTH) == MAX_STRING_LENGTH ||
        bounded_length(value, MAX_STRING_LENGTH) == MAX_STRING_LENGTH) {
        return -1;
    }

    index = hash_key(key, dict->bucket_count);
    current = dict->buckets[index];

    while (current != NULL) {
        if (strncmp(current->key, key, MAX_STRING_LENGTH) == 0) {
            value_copy = duplicate_string(value);
            if (value_copy == NULL) {
                return -1;
            }
            free(current->value);
            current->value = value_copy;
            return 0;
        }
        current = current->next;
    }

    node = malloc(sizeof(*node));
    if (node == NULL) {
        return -1;
    }

    key_copy = duplicate_string(key);
    if (key_copy == NULL) {
        free(node);
        return -1;
    }

    value_copy = duplicate_string(value);
    if (value_copy == NULL) {
        free(key_copy);
        free(node);
        return -1;
    }

    node->key = key_copy;
    node->value = value_copy;
    node->next = dict->buckets[index];
    dict->buckets[index] = node;
    dict->size++;

    return 0;
}

static int dict_is_empty(const Dictionary *dict)
{
    if (dict == NULL) {
        return 1;
    }

    return dict->size == 0;
}

static void dict_destroy(Dictionary *dict)
{
    DictNode *current;
    DictNode *next;

    if (dict == NULL) {
        return;
    }

    if (dict->buckets != NULL) {
        for (size_t i = 0; i < dict->bucket_count; i++) {
            current = dict->buckets[i];
            while (current != NULL) {
                next = current->next;
                free(current->key);
                free(current->value);
                free(current);
                current = next;
            }
            dict->buckets[i] = NULL;
        }
        free(dict->buckets);
    }

    free(dict);
}

int main(void)
{
    Dictionary *dict = dict_create(16);

    if (dict == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return EXIT_FAILURE;
    }

    printf("Dictionary is %s\n", dict_is_empty(dict) ? "empty" : "not empty");

    if (dict_insert(dict, "name", "Alice") != 0) {
        fprintf(stderr, "Failed to insert item\n");
        dict_destroy(dict);
        return EXIT_FAILURE;
    }

    printf("Dictionary is %s\n", dict_is_empty(dict) ? "empty" : "not empty");

    dict_destroy(dict);

    return EXIT_SUCCESS;
}