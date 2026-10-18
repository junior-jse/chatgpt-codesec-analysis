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
    int written;

    if (dest == NULL || dest_size == 0 || src == NULL) {
        return -1;
    }

    if (strnlen(src, dest_size) >= dest_size) {
        dest[0] = '\0';
        return -1;
    }

    written = snprintf(dest, dest_size, "%s", src);

    if (written < 0 || (size_t)written >= dest_size) {
        dest[0] = '\0';
        return -1;
    }

    return 0;
}

int addEntry(Dictionary *dict, const char *key, int value) {
    if (dict == NULL || key == NULL || dict->count >= MAX_ENTRIES) {
        return -1;
    }

    if (copyString(dict->entries[dict->count].key,
                   sizeof(dict->entries[dict->count].key),
                   key) != 0) {
        return -1;
    }

    dict->entries[dict->count].value = value;
    dict->count++;

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

    for (i = 0; i < dict->count; i++) {
        if (strcmp(dict->entries[i].key, key) == 0) {
            return (int)i;
        }
    }

    return -1;
}

int combineDictionaries(const Dictionary *dict1, const Dictionary *dict2, Dictionary *result) {
    size_t i;

    if (dict1 == NULL || dict2 == NULL || result == NULL) {
        return -1;
    }

    if (result == dict1 || result == dict2) {
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
            int a = result->entries[index].value;
            int b = dict2->entries[i].value;

            if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
                initDictionary(result);
                return -1;
            }

            result->entries[index].value = a + b;
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
    for (i = 0; i < dict->count; i++) {
        printf("  \"%.*s\": %d", MAX_KEY_LENGTH - 1, dict->entries[i].key, dict->entries[i].value);
        if (i < dict->count - 1) {
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