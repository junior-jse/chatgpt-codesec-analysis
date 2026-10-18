#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

typedef struct tuple {
    int32_t first;
    int32_t second;
    struct tuple *next;
} tuple_t;

static char *append_to_result(char *result, size_t *current_len, size_t *capacity,
                              const char *text, size_t text_len) {
    size_t i;
    size_t required;
    char *new_result;

    if (text_len == 0) {
        return result;
    }

    if (*current_len > SIZE_MAX - text_len - 1) {
        return NULL;
    }
    required = *current_len + text_len + 1;

    if (required > *capacity) {
        size_t new_capacity;
        if (*capacity > SIZE_MAX / 2) {
            new_capacity = required;
        } else {
            new_capacity = *capacity * 2;
            if (new_capacity < required) {
                new_capacity = required;
            }
        }
        new_result = (char*)realloc(result, new_capacity);
        if (new_result == NULL) {
            return NULL;
        }
        result = new_result;
        *capacity = new_capacity;
    }

    for (i = 0; i < text_len; i++) {
        result[*current_len + i] = text[i];
    }
    *current_len += text_len;
    result[*current_len] = '\0';
    return result;
}

static char *append_int(char **result, size_t *current_len, size_t *capacity, int32_t value) {
    int needed = snprintf(NULL, 0, "%d", value);
    size_t len;
    size_t required;
    char *new_res;
    int written;

    if (needed < 0) {
        return NULL;
    }
    len = (size_t)needed;

    if (*current_len > SIZE_MAX - len - 1) {
        return NULL;
    }
    required = *current_len + len + 1;

    if (required > *capacity) {
        size_t new_cap;
        if (*capacity > SIZE_MAX / 2) {
            new_cap = required;
        } else {
            new_cap = *capacity * 2;
            if (new_cap < required) {
                new_cap = required;
            }
        }
        new_res = (char*)realloc(*result, new_cap);
        if (new_res == NULL) {
            return NULL;
        }
        *result = new_res;
        *capacity = new_cap;
    }

    written = snprintf(*result + *current_len, len + 1, "%d", value);
    if (written < 0 || (size_t)written != len) {
        return NULL;
    }
    *current_len += len;
    return *result;
}

char *flatten_tuple_list(tuple_t *head) {
    char *result;
    size_t capacity;
    size_t current_len;
    tuple_t *current;
    int first_iteration;

    if (head == NULL) {
        char *empty = (char*)malloc(3);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '[';
        empty[1] = ']';
        empty[2] = '\0';
        return empty;
    }

    capacity = 64;
    result = (char*)malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    current_len = 0;
    result[0] = '\0';

    if (append_to_result(result, &current_len, &capacity, "[", 1) == NULL) {
        free(result);
        return NULL;
    }

    current = head;
    first_iteration = 1;

    while (current != NULL) {
        if (!first_iteration) {
            if (append_to_result(result, &current_len, &capacity, ", ", 2) == NULL) {
                free(result);
                return NULL;
            }
        }
        first_iteration = 0;

        if (append_to_result(result, &current_len, &capacity, "(", 1) == NULL) {
            free(result);
            return NULL;
        }

        if (append_int(&result, &current_len, &capacity, current->first) == NULL) {
            free(result);
            return NULL;
        }

        if (append_to_result(result, &current_len, &capacity, ", ", 2) == NULL) {
            free(result);
            return NULL;
        }

        if (append_int(&result, &current_len, &capacity, current->second) == NULL) {
            free(result);
            return NULL;
        }

        if (append_to_result(result, &current_len, &capacity, ")", 1) == NULL) {
            free(result);
            return NULL;
        }

        current = current->next;
    }

    if (append_to_result(result, &current_len, &capacity, "]", 1) == NULL) {
        free(result);
        return NULL;
    }

    return result;
}

int main(void) {
    tuple_t t3 = {30, 40, NULL};
    tuple_t t2 = {20, 30, &t3};
    tuple_t t1 = {10, 20, &t2};
    tuple_t single = {5, 6, NULL};
    char *flat;
    char *flat_single;
    char *flat_empty;

    flat = flatten_tuple_list(&t1);
    if (flat == NULL) {
        printf("Error: failed to flatten list\n");
        return 1;
    }
    printf("%s\n", flat);
    free(flat);

    flat_single = flatten_tuple_list(&single);
    if (flat_single == NULL) {
        printf("Error: failed to flatten single\n");
        return 1;
    }
    printf("%s\n", flat_single);
    free(flat_single);

    flat_empty = flatten_tuple_list(NULL);
    if (flat_empty == NULL) {
        printf("Error: failed to flatten empty\n");
        return 1;
    }
    printf("%s\n", flat_empty);
    free(flat_empty);

    return 0;
}