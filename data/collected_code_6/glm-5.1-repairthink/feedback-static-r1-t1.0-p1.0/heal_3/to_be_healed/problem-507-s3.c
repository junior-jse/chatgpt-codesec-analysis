#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char** remove_words(char** list, int* count, const char* word) {
    if (!list || !count || !word) {
        return list;
    }

    int new_count = 0;
    for (int i = 0; i < *count; i++) {
        if (list[i] && strcmp(list[i], word) == 0) {
            free(list[i]);
            list[i] = NULL;
        } else {
            list[new_count++] = list[i];
        }
    }

    for (int i = new_count; i < *count; i++) {
        list[i] = NULL;
    }

    /* Possible weaknesses found:
     *  *count is assigned 'new_count' here.
     */
    *count = new_count;

    /* Possible weaknesses found:
     *  Assuming condition is false
     */
    if (new_count == 0) {
        free(list);
        return NULL;
    }

    /* Possible weaknesses found:
     *  Condition 'new_count<*count' is always false [knownConditionTrueFalse]
     *  Condition 'new_count<*count' is always false
     */
    if (new_count < *count) {
        char** temp = realloc(list, new_count * sizeof(char*));
        if (temp) {
            list = temp;
        }
    }

    return list;
}

char** create_list(const char* items[], int size) {
    if (!items || size <= 0) {
        return NULL;
    }

    char** list = malloc(size * sizeof(char*));
    if (!list) {
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        if (!items[i]) {
            list[i] = NULL;
        } else {
            size_t len = strnlen(items[i], SIZE_MAX - 1);
            list[i] = malloc(len + 1);
            if (!list[i]) {
                for (int j = 0; j < i; j++) {
                    free(list[j]);
                }
                free(list);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(list[i], items[i], len);
            list[i][len] = '\0';
        }
    }
    return list;
}

void free_list(char** list, int count) {
    if (!list) {
        return;
    }
    for (int i = 0; i < count; i++) {
        free(list[i]);
    }
    free(list);
}

int main(void) {
    const char* original[] = {"apple", "banana", "cherry", "banana", "date"};
    int count = 5;

    char** list = create_list(original, count);
    if (!list) {
        return 1;
    }

    list = remove_words(list, &count, "banana");
    
    for (int i = 0; i < count; i++) {
        if (list[i]) {
            printf("%s\n", list[i]);
        }
    }

    free_list(list, count);

    return 0;
}