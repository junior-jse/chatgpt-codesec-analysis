#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static unsigned long hash_key(const char *key, size_t bucket_count)
{
    unsigned long hash = 5381UL;
    int c;

    if (key == NULL || bucket_count == 0) {
        return 0UL;
    }

    while ((c = (unsigned char)*key++) != '\0') {
        hash = ((hash << 5) + hash) + (unsigned long)c;
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

    len = strlen(s) + 1;
    copy = malloc(len);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, s, len);
    return copy;
}

static int dict_insert(Dictionary *dict, const char *key, const char *value)
{
    DictNode *node;
    DictNode *current;
    unsigned long index;
    char *key_copy;
    char *value_copy;

    if (dict == NULL || key == NULL || value == NULL || dict->bucket_count == 0) {
        return -1;
    }

    index = hash_key(key, dict->bucket_count);
    current = dict->buckets[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
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
    size_t i;
    DictNode *current;
    DictNode *next;

    if (dict == NULL) {
        return;
    }

    if (dict->buckets != NULL) {
        for (i = 0; i < dict->bucket_count; i++) {
            current = dict->buckets[i];
            while (current != NULL) {
                next = current->next;
                free(current->key);
                free(current->value);
                free(current);
                current = next;
            }
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