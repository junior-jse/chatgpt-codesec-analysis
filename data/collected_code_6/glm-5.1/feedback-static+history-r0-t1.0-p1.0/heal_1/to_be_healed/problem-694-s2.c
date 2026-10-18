#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    /* Possible weaknesses found:
     *  struct member 'DictionaryEntry::key' is never used. [unusedStructMember]
     */
    char *key;
    int value;
} DictionaryEntry;

typedef struct {
    int *data;
    size_t size;
} IntArray;

IntArray extract_unique_values(const DictionaryEntry *dict, size_t dict_size) {
    IntArray result;
    result.data = NULL;
    result.size = 0;

    if (dict == NULL || dict_size == 0) {
        return result;
    }

    result.data = (int *)malloc(dict_size * sizeof(int));
    if (result.data == NULL) {
        return result;
    }

    for (size_t i = 0; i < dict_size; i++) {
        int is_unique = 1;
        for (size_t j = 0; j < result.size; j++) {
            if (dict[i].value == result.data[j]) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            result.data[result.size++] = dict[i].value;
        }
    }

    if (result.size == 0) {
        free(result.data);
        result.data = NULL;
        return result;
    }

    int *temp = (int *)realloc(result.data, result.size * sizeof(int));
    if (temp != NULL) {
        result.data = temp;
    }

    return result;
}

int main(void) {
    DictionaryEntry dict[] = {
        {"a", 1},
        {"b", 2},
        {"c", 1},
        {"d", 3},
        {"e", 2},
        {"f", 4}
    };
    size_t dict_size = sizeof(dict) / sizeof(dict[0]);

    IntArray unique_values = extract_unique_values(dict, dict_size);

    for (size_t i = 0; i < unique_values.size; i++) {
        printf("%d ", unique_values.data[i]);
    }
    printf("\n");

    free(unique_values.data);
    unique_values.data = NULL;

    return 0;
}