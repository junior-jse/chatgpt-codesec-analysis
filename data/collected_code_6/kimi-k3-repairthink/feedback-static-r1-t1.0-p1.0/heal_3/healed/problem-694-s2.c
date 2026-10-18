#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICT_SIZE 100
#define MAX_KEY_LEN 50
#define MAX_VALUE_LEN 50

typedef struct {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
} DictionaryEntry;

static int safe_copy(char *dest, size_t dest_size, const char *src) {
    size_t src_len;

    if (dest == NULL || src == NULL || dest_size == 0) {
        return -1;
    }

    src_len = strlen(src);
    if (src_len >= dest_size) {
        dest[0] = '\0';
        return -1;
    }

    memcpy(dest, src, src_len + 1);
    return 0;
}

static int init_entry(DictionaryEntry *entry, const char *key, const char *value) {
    if (entry == NULL || key == NULL || value == NULL) {
        return -1;
    }

    if (safe_copy(entry->key, MAX_KEY_LEN, key) != 0) {
        return -1;
    }

    if (safe_copy(entry->value, MAX_VALUE_LEN, value) != 0) {
        return -1;
    }

    return 0;
}

int is_unique(const char values[][MAX_VALUE_LEN], int count, const char *value) {
    int i;

    if (values == NULL || value == NULL || count < 0) {
        return 0;
    }

    for (i = 0; i < count; i++) {
        if (strncmp(values[i], value, MAX_VALUE_LEN) == 0) {
            return 0;
        }
    }

    return 1;
}

int extract_unique_values(const DictionaryEntry *dict, int dict_size,
                          char unique_values[][MAX_VALUE_LEN], int max_unique) {
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
        if (is_unique((const char (*)[MAX_VALUE_LEN])unique_values, unique_count, dict[i].value)) {
            if (safe_copy(unique_values[unique_count], MAX_VALUE_LEN, dict[i].value) == 0) {
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

    DictionaryEntry dict[MAX_DICT_SIZE];
    char unique_values[MAX_DICT_SIZE][MAX_VALUE_LEN];
    size_t raw_count = sizeof(raw_entries) / sizeof(raw_entries[0]);
    int dict_size = 0;
    int unique_count;
    int i;
    int j;

    if (raw_count > (size_t)MAX_DICT_SIZE) {
        fprintf(stderr, "Dictionary size exceeds maximum capacity\n");
        return EXIT_FAILURE;
    }

    memset(dict, 0, sizeof(dict));
    memset(unique_values, 0, sizeof(unique_values));

    for (i = 0; i < (int)raw_count; i++) {
        if (init_entry(&dict[dict_size], raw_entries[i].key, raw_entries[i].value) != 0) {
            fprintf(stderr, "Entry %d exceeds buffer limits\n", i);
            return EXIT_FAILURE;
        }
        dict_size++;
    }

    unique_count = extract_unique_values(dict, dict_size, unique_values, MAX_DICT_SIZE);

    if (unique_count < 0) {
        fprintf(stderr, "Error extracting unique values\n");
        return EXIT_FAILURE;
    }

    printf("Unique values:\n");
    for (i = 0; i < unique_count; i++) {
        const char *first_key = "";
        for (j = 0; j < dict_size; j++) {
            if (strncmp(dict[j].value, unique_values[i], MAX_VALUE_LEN) == 0) {
                first_key = dict[j].key;
                break;
            }
        }
        printf("%s (first used with key: %s)\n", unique_values[i], first_key);
    }

    return EXIT_SUCCESS;
}