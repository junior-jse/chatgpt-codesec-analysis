#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICT_SIZE 100
#define MAX_KEY_LEN 50
#define MAX_VALUE_LEN 50

typedef struct {
    char *key;
    char *value;
} DictionaryEntry;

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;

    if (s == NULL) {
        return 0;
    }

    while (len < max_len && s[len] != '\0') {
        len++;
    }

    return len;
}

static char *bounded_strdup(const char *src, size_t max_len) {
    size_t src_len;
    size_t i;
    char *copy;

    if (src == NULL || max_len == 0) {
        return NULL;
    }

    src_len = bounded_strlen(src, max_len);
    if (src_len >= max_len) {
        return NULL;
    }

    copy = malloc(src_len + 1);
    if (copy == NULL) {
        return NULL;
    }

    for (i = 0; i < src_len; i++) {
        copy[i] = src[i];
    }
    copy[src_len] = '\0';

    return copy;
}

static void free_entry(DictionaryEntry *entry) {
    if (entry == NULL) {
        return;
    }

    free(entry->key);
    free(entry->value);
    entry->key = NULL;
    entry->value = NULL;
}

static int init_entry(DictionaryEntry *entry, const char *key, const char *value) {
    if (entry == NULL || key == NULL || value == NULL) {
        return -1;
    }

    entry->key = NULL;
    entry->value = NULL;

    entry->key = bounded_strdup(key, MAX_KEY_LEN);
    if (entry->key == NULL) {
        return -1;
    }

    entry->value = bounded_strdup(value, MAX_VALUE_LEN);
    if (entry->value == NULL) {
        free_entry(entry);
        return -1;
    }

    return 0;
}

int is_unique(char **values, int count, const char *value) {
    int i;

    if (values == NULL || value == NULL || count < 0) {
        return 0;
    }

    if (bounded_strlen(value, MAX_VALUE_LEN) >= MAX_VALUE_LEN) {
        return 0;
    }

    for (i = 0; i < count; i++) {
        if (values[i] != NULL && strncmp(values[i], value, MAX_VALUE_LEN) == 0) {
            return 0;
        }
    }

    return 1;
}

int extract_unique_values(const DictionaryEntry *dict, int dict_size,
                          char **unique_values, int max_unique) {
    int unique_count = 0;
    int i;

    if (dict == NULL || unique_values == NULL || dict_size <= 0 || max_unique <= 0) {
        return -1;
    }

    if (dict_size > MAX_DICT_SIZE || max_unique > MAX_DICT_SIZE) {
        return -1;
    }

    for (i = 0; i < dict_size; i++) {
        if (unique_count >= max_unique) {
            break;
        }

        if (dict[i].value == NULL) {
            continue;
        }

        if (bounded_strlen(dict[i].value, MAX_VALUE_LEN) >= MAX_VALUE_LEN) {
            continue;
        }

        if (is_unique(unique_values, unique_count, dict[i].value)) {
            unique_values[unique_count] = bounded_strdup(dict[i].value, MAX_VALUE_LEN);
            if (unique_values[unique_count] != NULL) {
                unique_count++;
            }
        }
    }

    return unique_count;
}

int main(void) {
    static const struct {
        const char *key;
        const char *value;
    } raw_entries[] = {
        {"key1", "value1"},
        {"key2", "value2"},
        {"key3", "value1"},
        {"key4", "value3"},
        {"key5", "value2"}
    };

    DictionaryEntry *dict = NULL;
    char **unique_values = NULL;
    size_t raw_count = sizeof(raw_entries) / sizeof(raw_entries[0]);
    int dict_size = 0;
    int unique_count = 0;
    int status = EXIT_FAILURE;
    int i;
    int j;

    if (raw_count > (size_t)MAX_DICT_SIZE) {
        fprintf(stderr, "Dictionary size exceeds maximum capacity\n");
        return EXIT_FAILURE;
    }

    dict = calloc(MAX_DICT_SIZE, sizeof(*dict));
    if (dict == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    unique_values = calloc(MAX_DICT_SIZE, sizeof(*unique_values));
    if (unique_values == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(dict);
        return EXIT_FAILURE;
    }

    for (i = 0; i < (int)raw_count; i++) {
        if (raw_entries[i].key == NULL || raw_entries[i].value == NULL) {
            fprintf(stderr, "Entry %d is invalid\n", i);
            goto cleanup;
        }
        if (init_entry(&dict[dict_size], raw_entries[i].key, raw_entries[i].value) != 0) {
            fprintf(stderr, "Entry %d exceeds buffer limits\n", i);
            goto cleanup;
        }
        dict_size++;
    }

    unique_count = extract_unique_values(dict, dict_size, unique_values, MAX_DICT_SIZE);
    if (unique_count < 0) {
        fprintf(stderr, "Error extracting unique values\n");
        goto cleanup;
    }

    printf("Unique values:\n");
    for (i = 0; i < unique_count; i++) {
        const char *first_key = "";
        for (j = 0; j < dict_size; j++) {
            if (dict[j].value != NULL && unique_values[i] != NULL &&
                strncmp(dict[j].value, unique_values[i], MAX_VALUE_LEN) == 0) {
                first_key = dict[j].key;
                break;
            }
        }
        printf("%s (first used with key: %s)\n", unique_values[i], first_key);
    }

    status = EXIT_SUCCESS;

cleanup:
    if (dict != NULL) {
        for (i = 0; i < MAX_DICT_SIZE; i++) {
            free_entry(&dict[i]);
        }
        free(dict);
    }
    if (unique_values != NULL) {
        for (i = 0; i < MAX_DICT_SIZE; i++) {
            free(unique_values[i]);
        }
        free(unique_values);
    }

    return status;
}