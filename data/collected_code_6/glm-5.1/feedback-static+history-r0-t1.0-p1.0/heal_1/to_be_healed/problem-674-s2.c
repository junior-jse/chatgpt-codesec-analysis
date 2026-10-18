#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char **items;
    size_t size;
    size_t capacity;
} Collection;

void collection_init(Collection *col) {
    col->items = malloc(8 * sizeof(char *));
    if (col->items == NULL) {
        exit(EXIT_FAILURE);
    }
    col->size = 0;
    col->capacity = 8;
}

void collection_append(Collection *col, const char *item) {
    if (col->size == col->capacity) {
        col->capacity *= 2;
        char **new_items = realloc(col->items, col->capacity * sizeof(char *));
        if (new_items == NULL) {
            exit(EXIT_FAILURE);
        }
        col->items = new_items;
    }
    col->items[col->size] = strdup(item);
    if (col->items[col->size] == NULL) {
        exit(EXIT_FAILURE);
    }
    col->size++;
}

bool collection_contains(Collection *col, const char *item) {
    for (size_t i = 0; i < col->size; i++) {
        if (strcmp(col->items[i], item) == 0) {
            return true;
        }
    }
    return false;
}

void collection_free(Collection *col) {
    for (size_t i = 0; i < col->size; i++) {
        free(col->items[i]);
    }
    free(col->items);
    col->items = NULL;
    col->size = 0;
    col->capacity = 0;
}

char *remove_duplicate_words(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    Collection seen;
    collection_init(&seen);

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t result_cap = strlen(str) + 1;
    char *result = malloc(result_cap);
    if (result == NULL) {
        exit(EXIT_FAILURE);
    }
    result[0] = '\0';

    char *temp_str = strdup(str);
    if (temp_str == NULL) {
        exit(EXIT_FAILURE);
    }

    const char *delim = " \t\n\r";
    char *token = strtok(temp_str, delim);
    char *current_ptr = result;

    while (token != NULL) {
        if (!collection_contains(&seen, token)) {
            collection_append(&seen, token);
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t token_len = strlen(token);
            if (current_ptr != result) {
                *current_ptr = ' ';
                current_ptr++;
                *current_ptr = '\0';
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(current_ptr, token, token_len);
            current_ptr += token_len;
            *current_ptr = '\0';
        }
        token = strtok(NULL, delim);
    }

    free(temp_str);
    collection_free(&seen);

    char *final_result = strdup(result);
    free(result);
    if (final_result == NULL) {
        exit(EXIT_FAILURE);
    }
    
    return final_result;
}

int main() {
    /* Possible weaknesses found:
     *  Variable 'input' can be declared as const array [constVariable]
     */
    char input[] = "hello world hello universe world again";
    char *output = remove_duplicate_words(input);

    if (output != NULL) {
        printf("%s\n", output);
        free(output);
    }

    return 0;
}