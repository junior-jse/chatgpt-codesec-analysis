#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 4096

typedef struct {
    char* key;
    char* value;
} KeyValuePair;

typedef struct {
    KeyValuePair* pairs;
    size_t size;
    size_t capacity;
} Dictionary;

Dictionary* create_dictionary(size_t capacity) {
    Dictionary* dict = (Dictionary*)malloc(sizeof(Dictionary));
    if (dict == NULL) {
        return NULL;
    }
    dict->pairs = (KeyValuePair*)malloc(capacity * sizeof(KeyValuePair));
    if (dict->pairs == NULL) {
        free(dict);
        return NULL;
    }
    dict->size = 0;
    dict->capacity = capacity;
    return dict;
}

void free_dictionary(Dictionary* dict) {
    if (dict == NULL) {
        return;
    }
    for (size_t i = 0; i < dict->size; i++) {
        free(dict->pairs[i].key);
        free(dict->pairs[i].value);
    }
    free(dict->pairs);
    free(dict);
}

static size_t bounded_strlen(const char* s, size_t max_len) {
    size_t len = 0;
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static char* safe_string_copy(const char* src) {
    size_t len = bounded_strlen(src, MAX_STRING_LENGTH);
    if (len == MAX_STRING_LENGTH) {
        return NULL;
    }
    size_t dest_size = len + 1;
    char* dest = (char*)malloc(dest_size);
    if (dest == NULL) {
        return NULL;
    }
    if (len >= dest_size) {
        free(dest);
        return NULL;
    }
    int written = snprintf(dest, dest_size, "%s", src);
    if (written < 0 || (size_t)written >= dest_size) {
        free(dest);
        return NULL;
    }
    return dest;
}

int add_pair(Dictionary* dict, const char* key, const char* value) {
    if (dict == NULL || key == NULL || value == NULL) {
        return -1;
    }
    if (dict->size >= dict->capacity) {
        return -1;
    }
    dict->pairs[dict->size].key = safe_string_copy(key);
    if (dict->pairs[dict->size].key == NULL) {
        return -1;
    }
    dict->pairs[dict->size].value = safe_string_copy(value);
    if (dict->pairs[dict->size].value == NULL) {
        free(dict->pairs[dict->size].key);
        dict->pairs[dict->size].key = NULL;
        return -1;
    }
    dict->size++;
    return 0;
}

Dictionary* tuple_to_dict(const char* const tuple[], size_t tuple_size) {
    if (tuple == NULL || tuple_size == 0) {
        return NULL;
    }
    size_t pair_count = tuple_size / 2;
    Dictionary* dict = create_dictionary(pair_count);
    if (dict == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < pair_count; i++) {
        if (add_pair(dict, tuple[i * 2], tuple[i * 2 + 1]) != 0) {
            free_dictionary(dict);
            return NULL;
        }
    }
    return dict;
}

const char* get_value(Dictionary* dict, const char* key) {
    if (dict == NULL || key == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < dict->size; i++) {
        if (strcmp(dict->pairs[i].key, key) == 0) {
            return dict->pairs[i].value;
        }
    }
    return NULL;
}

void print_dictionary(Dictionary* dict) {
    if (dict == NULL) {
        return;
    }
    printf("{");
    for (size_t i = 0; i < dict->size; i++) {
        printf("'%s': '%s'", dict->pairs[i].key, dict->pairs[i].value);
        if (i < dict->size - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

int main(void) {
    const char* tuple[] = {"name", "John", "age", "30", "city", "New York"};
    size_t tuple_size = sizeof(tuple) / sizeof(tuple[0]);

    Dictionary* dict = tuple_to_dict(tuple, tuple_size);
    if (dict == NULL) {
        fprintf(stderr, "Failed to create dictionary\n");
        return EXIT_FAILURE;
    }

    printf("Dictionary: ");
    print_dictionary(dict);

    const char* value = get_value(dict, "age");
    if (value != NULL) {
        printf("Value for key 'age': %s\n", value);
    }

    free_dictionary(dict);
    return EXIT_SUCCESS;
}