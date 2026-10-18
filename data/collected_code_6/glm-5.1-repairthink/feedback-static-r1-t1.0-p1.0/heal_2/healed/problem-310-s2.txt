#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 1048576

static size_t safe_strnlen(const char *s, size_t maxlen) {
    size_t len = 0;
    while (len < maxlen && s[len] != '\0') {
        len++;
    }
    return len;
}

typedef struct {
    char **items;
    size_t count;
} Tuple;

Tuple *string_to_tuple(const char *str, const char *delim) {
    if (str == NULL || delim == NULL) {
        return NULL;
    }

    Tuple *tuple = malloc(sizeof(Tuple));
    if (tuple == NULL) {
        return NULL;
    }

    tuple->items = NULL;
    tuple->count = 0;

    size_t str_len = safe_strnlen(str, MAX_STR_LEN + 1);
    if (str_len == MAX_STR_LEN + 1) {
        free(tuple);
        return NULL;
    }

    char *temp = malloc(str_len + 1);
    if (temp == NULL) {
        free(tuple);
        return NULL;
    }
    memcpy(temp, str, str_len + 1);

    char *token = strtok(temp, delim);
    while (token != NULL) {
        char **new_items = realloc(tuple->items, (tuple->count + 1) * sizeof(char *));
        if (new_items == NULL) {
            for (size_t i = 0; i < tuple->count; i++) {
                free(tuple->items[i]);
            }
            free(tuple->items);
            free(tuple);
            free(temp);
            return NULL;
        }
        tuple->items = new_items;

        size_t token_len = safe_strnlen(token, str_len + 1);

        tuple->items[tuple->count] = malloc(token_len + 1);
        if (tuple->items[tuple->count] == NULL) {
            for (size_t i = 0; i < tuple->count; i++) {
                free(tuple->items[i]);
            }
            free(tuple->items);
            free(tuple);
            free(temp);
            return NULL;
        }
        memcpy(tuple->items[tuple->count], token, token_len + 1);
        tuple->count++;

        token = strtok(NULL, delim);
    }

    free(temp);
    return tuple;
}

void free_tuple(Tuple *tuple) {
    if (tuple != NULL) {
        for (size_t i = 0; i < tuple->count; i++) {
            free(tuple->items[i]);
        }
        free(tuple->items);
        free(tuple);
    }
}

int main(void) {
    const char *input = "apple,banana,cherry";
    const char *delim = ",";

    Tuple *my_tuple = string_to_tuple(input, delim);
    if (my_tuple == NULL) {
        return EXIT_FAILURE;
    }

    printf("(");
    for (size_t i = 0; i < my_tuple->count; i++) {
        printf("'%s'", my_tuple->items[i]);
        if (i < my_tuple->count - 1) {
            printf(", ");
        }
    }
    printf(")\n");

    free_tuple(my_tuple);

    return EXIT_SUCCESS;
}