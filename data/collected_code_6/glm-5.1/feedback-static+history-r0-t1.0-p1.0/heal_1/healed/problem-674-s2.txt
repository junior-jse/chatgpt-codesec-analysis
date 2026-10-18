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

    size_t str_len = strnlen(str, SIZE_MAX);
    size_t result_cap = str_len + 1;
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
    char *saveptr;
    char *token = strtok_r(temp_str, delim, &saveptr);
    char *current_ptr = result;

    while (token != NULL) {
        if (!collection_contains(&seen, token)) {
            collection_append(&seen, token);
            size_t token_len = strnlen(token, SIZE_MAX);
            
            size_t required_len = (current_ptr - result) + token_len + 2;
            if (required_len > result_cap) {
                size_t offset = current_ptr - result;
                result_cap = required_len * 2;
                char *new_result = realloc(result, result_cap);
                if (new_result == NULL) {
                    free(result);
                    free(temp_str);
                    collection_free(&seen);
                    exit(EXIT_FAILURE);
                }
                result = new_result;
                current_ptr = result + offset;
            }

            if (current_ptr != result) {
                *current_ptr = ' ';
                current_ptr++;
                *current_ptr = '\0';
            }
            memcpy(current_ptr, token, token_len);
            current_ptr += token_len;
            *current_ptr = '\0';
        }
        token = strtok_r(NULL, delim, &saveptr);
    }

    free(temp_str);
    collection_free(&seen);

    size_t final_len = strnlen(result, result_cap);
    char *final_result = malloc(final_len + 1);
    if (final_result == NULL) {
        free(result);
        exit(EXIT_FAILURE);
    }
    memcpy(final_result, result, final_len + 1);
    
    free(result);
    return final_result;
}

int main() {
    const char * const input = "hello world hello universe world again";
    char *output = remove_duplicate_words(input);

    if (output != NULL) {
        printf("%s\n", output);
        free(output);
    }

    return 0;
}