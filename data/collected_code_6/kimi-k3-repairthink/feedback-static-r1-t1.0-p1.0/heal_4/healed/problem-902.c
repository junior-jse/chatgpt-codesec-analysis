#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_KEY_LENGTH 50
#define MAX_ENTRIES 100

typedef struct {
    char key[MAX_KEY_LENGTH];
    int value;
} DictEntry;

typedef struct {
    DictEntry entries[MAX_ENTRIES];
    size_t count;
} Dictionary;

void initDictionary(Dictionary *dict) {
    if (dict != NULL) {
        memset(dict, 0, sizeof(*dict));
    }
}

static int copyString(char *dest, size_t dest_size, const char *src) {
    size_t src_len;

    if (dest == NULL || dest_size == 0 || src == NULL) {
        return -1;
    }

    src_len = strnlen(src, dest_size);
    if (src_len >= dest_size) {
        dest[0] = '\0';
        return -1;
    }

    memcpy(dest, src, src_len + 1);
    return 0;
}

int addEntry(Dictionary *dict, const char *key, int value) {
    size_t slot;

    if (dict == NULL || key == NULL || dict->count >= MAX_ENTRIES) {
        return -1;
    }

    slot = dict->count;

    if (copyString(dict->entries[slot].key, sizeof(dict->entries[slot].key), key) != 0) {
        dict->entries[slot].key[0] = '\0';
        return -1;
    }

    dict->entries[slot].value = value;
    dict->count = slot + 1;

    return 0;
}

int findEntry(const Dictionary *dict, const char *key) {
    size_t i;

    if (dict == NULL || key == NULL) {
        return -1;
    }

    if (strnlen(key, MAX_KEY_LENGTH) >= MAX_KEY_LENGTH) {
        return -1;
    }

    for (i = 0; i < dict->count && i < MAX_ENTRIES; i++) {
        if (strncmp(dict->entries[i].key, key, MAX_KEY_LENGTH) == 0) {
            return (int)i;
        }
    }

    return -1;
}

static int checkedAdd(int a, int b, int *out) {
    if (out == NULL) {
        return -1;
    }

    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return -1;
    }

    *out = a + b;
    return 0;
}

int combineDictionaries(const Dictionary *dict1, const Dictionary *dict2, Dictionary *result) {
    size_t i;

    if (dict1 == NULL || dict2 == NULL || result == NULL) {
        return -1;
    }

    if (result == dict1 || result == dict2) {
        return -1;
    }

    if (dict1->count > MAX_ENTRIES || dict2->count > MAX_ENTRIES) {
        return -1;
    }

    initDictionary(result);

    for (i = 0; i < dict1->count; i++) {
        if (addEntry(result, dict1->entries[i].key, dict1->entries[i].value) != 0) {
            initDictionary(result);
            return -1;
        }
    }

    for (i = 0; i < dict2->count; i++) {
        int index = findEntry(result, dict2->entries[i].key);

        if (index >= 0) {
            int sum;

            if (checkedAdd(result->entries[index].value, dict2->entries[i].value, &sum) != 0) {
                initDictionary(result);
                return -1;
            }

            result->entries[index].value = sum;
        } else {
            if (addEntry(result, dict2->entries[i].key, dict2->entries[i].value) != 0) {
                initDictionary(result);
                return -1;
            }
        }
    }

    return 0;
}

void printDictionary(const Dictionary *dict) {
    size_t i;

    if (dict == NULL) {
        return;
    }

    printf("{\n");
    for (i = 0; i < dict->count && i < MAX_ENTRIES; i++) {
        printf("  \"%.*s\": %d", MAX_KEY_LENGTH - 1, dict->entries[i].key, dict->entries[i].value);
        if (i + 1 < dict->count && i + 1 < MAX_ENTRIES) {
            printf(",");
        }
        printf("\n");
    }
    printf("}\n");
}

int main(void) {
    Dictionary dict1, dict2, combined;

    initDictionary(&dict1);
    initDictionary(&dict2);
    initDictionary(&combined);

    if (addEntry(&dict1, "apple", 5) != 0 ||
        addEntry(&dict1, "banana", 3) != 0 ||
        addEntry(&dict1, "orange", 7) != 0) {
        fprintf(stderr, "Error adding entries to dict1\n");
        return EXIT_FAILURE;
    }

    if (addEntry(&dict2, "banana", 2) != 0 ||
        addEntry(&dict2, "grape", 4) != 0 ||
        addEntry(&dict2, "apple", 3) != 0) {
        fprintf(stderr, "Error adding entries to dict2\n");
        return EXIT_FAILURE;
    }

    printf("Dictionary 1:\n");
    printDictionary(&dict1);

    printf("\nDictionary 2:\n");
    printDictionary(&dict2);

    if (combineDictionaries(&dict1, &dict2, &combined) != 0) {
        fprintf(stderr, "Error combining dictionaries\n");
        return EXIT_FAILURE;
    }

    printf("\nCombined Dictionary:\n");
    printDictionary(&combined);

    return EXIT_SUCCESS;
}