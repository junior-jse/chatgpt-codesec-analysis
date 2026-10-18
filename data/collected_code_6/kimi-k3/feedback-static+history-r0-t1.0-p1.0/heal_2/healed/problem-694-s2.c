#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICT_SIZE 100
#define MAX_STR_LEN 50

typedef struct {
    char key[MAX_STR_LEN];
    char value[MAX_STR_LEN];
} DictionaryEntry;

int is_unique(const char values[][MAX_STR_LEN], int count, const char *value) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(values[i], value) == 0) {
            return 0;
        }
    }
    return 1;
}

int extract_unique_values(const DictionaryEntry *dict, int dict_size, char unique_values[][MAX_STR_LEN], int max_unique) {
    int unique_count = 0;
    int i;
    
    if (dict == NULL || unique_values == NULL || dict_size <= 0 || max_unique <= 0) {
        return -1;
    }
    
    for (i = 0; i < dict_size; i++) {
        if (unique_count >= max_unique) {
            break;
        }
        if (is_unique(unique_values, unique_count, dict[i].value)) {
            snprintf(unique_values[unique_count], MAX_STR_LEN, "%s", dict[i].value);
            unique_count++;
        }
    }
    
    return unique_count;
}

int main(void) {
    DictionaryEntry dict[] = {
        {"key1", "value1"},
        {"key2", "value2"},
        {"key3", "value1"},
        {"key4", "value3"},
        {"key5", "value2"}
    };
    
    int dict_size = sizeof(dict) / sizeof(dict[0]);
    char unique_values[MAX_DICT_SIZE][MAX_STR_LEN];
    int unique_count;
    int i;
    
    unique_count = extract_unique_values(dict, dict_size, unique_values, MAX_DICT_SIZE);
    
    if (unique_count < 0) {
        fprintf(stderr, "Error extracting unique values\n");
        return EXIT_FAILURE;
    }
    
    printf("Unique values:\n");
    for (i = 0; i < unique_count; i++) {
        printf("%s\n", unique_values[i]);
    }
    
    return EXIT_SUCCESS;
}