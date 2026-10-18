#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
} StringList;

static int is_valid_string(const char *str, size_t max_len) {
    size_t i;
    if (str == NULL) {
        return 0;
    }
    for (i = 0; i < max_len; i++) {
        if (str[i] == '\0') {
            return 1;
        }
    }
    return 0;
}

static size_t safe_strlen(const char *str, size_t max_len) {
    size_t i;
    if (str == NULL) {
        return 0;
    }
    for (i = 0; i < max_len; i++) {
        if (str[i] == '\0') {
            return i;
        }
    }
    return max_len;
}

char *join_tuple_with_string(char **tuple, size_t count, const char *separator) {
    size_t total_length;
    size_t separator_length;
    size_t i;
    char *result;
    char *current_pos;
    size_t remaining_space;

    if (tuple == NULL || separator == NULL || count == 0) {
        return NULL;
    }

    if (!is_valid_string(separator, 4096)) {
        return NULL;
    }

    separator_length = safe_strlen(separator, 4096);
    total_length = 1;

    for (i = 0; i < count; i++) {
        if (tuple[i] == NULL) {
            return NULL;
        }
        if (!is_valid_string(tuple[i], 4096)) {
            return NULL;
        }
        total_length += safe_strlen(tuple[i], 4096) + separator_length;
        if (total_length > 1024 * 1024) {
            return NULL;
        }
    }

    result = malloc(total_length);
    if (result == NULL) {
        return NULL;
    }

    current_pos = result;
    remaining_space = total_length;

    for (i = 0; i < count; i++) {
        size_t item_length = safe_strlen(tuple[i], 4096);
        
        if (item_length >= remaining_space) {
            free(result);
            return NULL;
        }
        
        if (item_length > 0) {
            memcpy(current_pos, tuple[i], item_length);
            current_pos += item_length;
            remaining_space -= item_length;
        }

        if (separator_length >= remaining_space) {
            free(result);
            return NULL;
        }
        
        if (separator_length > 0) {
            memcpy(current_pos, separator, separator_length);
            current_pos += separator_length;
            remaining_space -= separator_length;
        }
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

    if (!is_valid_string(separator, 4096)) {
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

    separator_length = safe_strlen(separator, 4096);

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

        if (!is_valid_string(tuple[i], 4096)) {
            size_t j;
            for (j = 0; j < i * 2; j++) {
                free(list->items[j]);
            }
            free(list->items);
            free(list);
            return NULL;
        }

        item_length = safe_strlen(tuple[i], 4096);
        
        if (item_length >= 4096) {
            size_t j;
            for (j = 0; j < i * 2; j++) {
                free(list->items[j]);
            }
            free(list->items);
            free(list);
            return NULL;
        }

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
        
        if (item_length > 0) {
            memcpy(list->items[i * 2], tuple[i], item_length);
        }
        list->items[i * 2][item_length] = '\0';

        if (separator_length >= 4096) {
            size_t j;
            for (j = 0; j <= i * 2; j++) {
                free(list->items[j]);
            }
            free(list->items);
            free(list);
            return NULL;
        }

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
        
        if (separator_length > 0) {
            memcpy(list->items[i * 2 + 1], separator, separator_length);
        }
        list->items[i * 2 + 1][separator_length] = '\0';
    }

    return list;
}

void free_string_list(StringList *list) {
    if (list == NULL) {
        return;
    }

    if (list->items != NULL) {
        size_t i;
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