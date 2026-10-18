#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
} StringList;

char *join_tuple_with_string(char **tuple, size_t count, const char *separator) {
    size_t total_length;
    size_t separator_length;
    size_t i;
    char *result;
    char *current_pos;

    if (tuple == NULL || separator == NULL || count == 0) {
        return NULL;
    }

    separator_length = strlen(separator);
    total_length = 1;

    for (i = 0; i < count; i++) {
        if (tuple[i] == NULL) {
            return NULL;
        }
        total_length += strlen(tuple[i]) + separator_length;
    }

    result = malloc(total_length);
    if (result == NULL) {
        return NULL;
    }

    current_pos = result;
    for (i = 0; i < count; i++) {
        size_t item_length = strlen(tuple[i]);
        memcpy(current_pos, tuple[i], item_length);
        current_pos += item_length;
        memcpy(current_pos, separator, separator_length);
        current_pos += separator_length;
    }
    *current_pos = '\0';

    return result;
}

StringList *tuple_to_list_with_string(char **tuple, size_t count, const char *separator) {
    StringList *list;
    size_t i;
    size_t separator_length;

    if (tuple == NULL || separator == NULL || count == 0) {
        return NULL;
    }

    list = malloc(sizeof(StringList));
    if (list == NULL) {
        return NULL;
    }

    list->count = count * 2;
    list->items = malloc(list->count * sizeof(char *));
    if (list->items == NULL) {
        free(list);
        return NULL;
    }

    separator_length = strlen(separator);

    for (i = 0; i < count; i++) {
        size_t item_length;

        if (tuple[i] == NULL) {
            size_t j;
            for (j = 0; j < i * 2; j++) {
                free(list->items[j]);
            }
            free(list->items);
            free(list);
            return NULL;
        }

        item_length = strlen(tuple[i]);
        list->items[i * 2] = malloc(item_length + 1);
        if (list->items[i * 2] == NULL) {
            size_t j;
            for (j = 0; j < i * 2; j++) {
                free(list->items[j]);
            }
            free(list->items);
            free(list);
            return NULL;
        }
        memcpy(list->items[i * 2], tuple[i], item_length + 1);

        list->items[i * 2 + 1] = malloc(separator_length + 1);
        if (list->items[i * 2 + 1] == NULL) {
            size_t j;
            for (j = 0; j <= i * 2; j++) {
                free(list->items[j]);
            }
            free(list->items);
            free(list);
            return NULL;
        }
        memcpy(list->items[i * 2 + 1], separator, separator_length + 1);
    }

    return list;
}

void free_string_list(StringList *list) {
    size_t i;

    if (list == NULL) {
        return;
    }

    if (list->items != NULL) {
        for (i = 0; i < list->count; i++) {
            free(list->items[i]);
        }
        free(list->items);
    }

    free(list);
}

int main(void) {
    char *tuple[] = {"apple", "banana", "cherry"};
    size_t tuple_count = 3;
    const char *separator = "fruit";
    StringList *result;
    size_t i;

    result = tuple_to_list_with_string(tuple, tuple_count, separator);
    if (result == NULL) {
        fprintf(stderr, "Error: Failed to convert tuple to list\n");
        return EXIT_FAILURE;
    }

    printf("[");
    for (i = 0; i < result->count; i++) {
        printf("\"%s\"", result->items[i]);
        if (i < result->count - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    free_string_list(result);

    return EXIT_SUCCESS;
}