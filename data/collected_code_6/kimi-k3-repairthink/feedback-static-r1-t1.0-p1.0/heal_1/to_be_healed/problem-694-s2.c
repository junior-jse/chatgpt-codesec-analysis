#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICT_SIZE 100

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     *  struct member 'DictionaryEntry::key' is never used. [unusedStructMember]
     */
    char key[50];
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char value[50];
} DictionaryEntry;

/* Possible weaknesses found:
 *  Parameter 'values' can be declared as const array [constParameter]
 */
int is_unique(char values[][50], int count, const char *value) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(values[i], value) == 0) {
            return 0;
        }
    }
    return 1;
}

int extract_unique_values(DictionaryEntry *dict, int dict_size, char unique_values[][50]) {
    int unique_count = 0;
    int i;
    
    if (dict == NULL || unique_values == NULL || dict_size <= 0) {
        return -1;
    }
    
    for (i = 0; i < dict_size; i++) {
        if (is_unique(unique_values, unique_count, dict[i].value)) {
            /* Possible weaknesses found:
             * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
             */
            strcpy(unique_values[unique_count], dict[i].value);
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
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char unique_values[MAX_DICT_SIZE][50];
    int unique_count;
    int i;
    
    unique_count = extract_unique_values(dict, dict_size, unique_values);
    
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