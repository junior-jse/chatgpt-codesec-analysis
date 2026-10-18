#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    char *key;
    char *value;
} DictEntry;

typedef struct {
    DictEntry *entries;
    size_t size;
    size_t capacity;
} Dictionary;

static char *dict_safe_strdup(const char *src) {
    size_t len;
    char *copy;

    if (src == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len = strlen(src);
    if (len == SIZE_MAX) {
        return NULL;
    }

    copy = (char *)malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, src, len);
    copy[len] = '\0';

    return copy;
}

int dict_init(Dictionary *dict, size_t capacity) {
    if (dict == NULL) {
        return -1;
    }
    dict->entries = NULL;
    dict->size = 0;
    dict->capacity = 0;

    if (capacity > 0) {
        if (capacity > SIZE_MAX / sizeof(DictEntry)) {
            return -1;
        }
        dict->entries = (DictEntry *)calloc(capacity, sizeof(DictEntry));
        if (dict->entries == NULL) {
            return -1;
        }
        dict->capacity = capacity;
    }
    return 0;
}

int dict_set(Dictionary *dict, size_t index, const char *key, const char *value) {
    char *new_key = NULL;
    char *new_value = NULL;

    if (dict == NULL || dict->entries == NULL || index >= dict->capacity) {
        return -1;
    }

    if (key != NULL) {
        new_key = dict_safe_strdup(key);
        if (new_key == NULL) {
            return -1;
        }
    }

    if (value != NULL) {
        new_value = dict_safe_strdup(value);
        if (new_value == NULL) {
            free(new_key);
            return -1;
        }
    }

    free(dict->entries[index].key);
    free(dict->entries[index].value);

    dict->entries[index].key = new_key;
    dict->entries[index].value = new_value;

    if (index >= dict->size) {
        dict->size = index + 1;
    }

    return 0;
}

void dict_free(Dictionary *dict) {
    /* Possible weaknesses found:
     *  The scope of the variable 'i' can be reduced. [variableScope]
     */
    size_t i;

    if (dict != NULL && dict->entries != NULL) {
        for (i = 0; i < dict->capacity; i++) {
            free(dict->entries[i].key);
            free(dict->entries[i].value);
        }
        free(dict->entries);
        dict->entries = NULL;
        dict->size = 0;
        dict->capacity = 0;
    }
}

int dict_is_empty(const Dictionary *dict) {
    if (dict == NULL) {
        return 1;
    }
    return dict->size == 0;
}

int all_dicts_empty(const Dictionary *dicts, size_t count) {
    size_t i;

    if (dicts == NULL || count == 0) {
        return 1;
    }

    for (i = 0; i < count; i++) {
        if (!dict_is_empty(&dicts[i])) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    Dictionary *dict_list = NULL;
    size_t dict_count = 3;
    size_t i;
    int result;

    dict_list = (Dictionary *)calloc(dict_count, sizeof(Dictionary));
    if (dict_list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    for (i = 0; i < dict_count; i++) {
        if (dict_init(&dict_list[i], 10) != 0) {
            size_t j;
            for (j = 0; j < i; j++) {
                dict_free(&dict_list[j]);
            }
            free(dict_list);
            fprintf(stderr, "Dictionary initialization failed\n");
            return EXIT_FAILURE;
        }
    }

    result = all_dicts_empty(dict_list, dict_count);
    printf("All dictionaries empty: %s\n", result ? "true" : "false");

    if (dict_set(&dict_list[1], 0, "test", "") != 0) {
        for (i = 0; i < dict_count; i++) {
            dict_free(&dict_list[i]);
        }
        free(dict_list);
        fprintf(stderr, "Failed to set dictionary entry\n");
        return EXIT_FAILURE;
    }

    result = all_dicts_empty(dict_list, dict_count);
    printf("All dictionaries empty: %s\n", result ? "true" : "false");

    for (i = 0; i < dict_count; i++) {
        dict_free(&dict_list[i]);
    }
    free(dict_list);

    return EXIT_SUCCESS;
}