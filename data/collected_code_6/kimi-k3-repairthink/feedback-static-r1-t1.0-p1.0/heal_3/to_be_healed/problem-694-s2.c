#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICT_SIZE 100
#define MAX_KEY_LEN 50
#define MAX_VALUE_LEN 50

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char key[MAX_KEY_LEN];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char value[MAX_VALUE_LEN];
} DictionaryEntry;

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

    for (i = 0; i < dict_size; i++) {
        if (unique_count >= max_unique) {
            break;
        }
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        if (is_unique((const char (*)[MAX_VALUE_LEN])unique_values, unique_count, dict[i].value)) {
            snprintf(unique_values[unique_count], MAX_VALUE_LEN, "%s", dict[i].value);
            unique_values[unique_count][MAX_VALUE_LEN - 1] = '\0';
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

    int dict_size = (int)(sizeof(dict) / sizeof(dict[0]));
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char unique_values[MAX_DICT_SIZE][MAX_VALUE_LEN];
    int unique_count;
    int i;
    int j;

    if (dict_size > MAX_DICT_SIZE) {
        fprintf(stderr, "Dictionary size exceeds maximum capacity\n");
        return EXIT_FAILURE;
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