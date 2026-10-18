#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct tuple {
    int32_t first;
    int32_t second;
    struct tuple *next;
} tuple_t;

static char *append_to_result(char *result, size_t *current_len, size_t *capacity,
                              const char *text, size_t text_len) {
    if (text_len == 0) {
        return result;
    }
    if (*current_len + text_len + 1 > *capacity) {
        size_t new_capacity = *capacity * 2;
        if (new_capacity < *current_len + text_len + 1) {
            new_capacity = *current_len + text_len + 1;
        }
        char *new_result = (char*)realloc(result, new_capacity);
        if (new_result == NULL) {
            return NULL;
        }
        result = new_result;
        *capacity = new_capacity;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(result + *current_len, text, text_len);
    *current_len += text_len;
    result[*current_len] = '\0';
    return result;
}

char *flatten_tuple_list(tuple_t *head) {
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

    size_t capacity = 64;
    char *result = (char*)malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    size_t current_len = 0;
    result[0] = '\0';

    if (append_to_result(result, &current_len, &capacity, "[", 1) == NULL) {
        free(result);
        return NULL;
    }

    tuple_t *current = head;
    int first_iteration = 1;

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

        int needed1 = snprintf(NULL, 0, "%d", current->first);
        if (needed1 < 0) {
            free(result);
            return NULL;
        }
        size_t len1 = (size_t)needed1;
        if (current_len + len1 + 1 > capacity) {
            size_t new_cap = capacity * 2;
            if (new_cap < current_len + len1 + 1) {
                new_cap = current_len + len1 + 1;
            }
            char *new_res = (char*)realloc(result, new_cap);
            if (new_res == NULL) {
                free(result);
                return NULL;
            }
            result = new_res;
            capacity = new_cap;
        }
        int written1 = snprintf(result + current_len, len1 + 1, "%d", current->first);
        if (written1 < 0 || (size_t)written1 != len1) {
            free(result);
            return NULL;
        }
        current_len += len1;

        if (append_to_result(result, &current_len, &capacity, ", ", 2) == NULL) {
            free(result);
            return NULL;
        }

        int needed2 = snprintf(NULL, 0, "%d", current->second);
        if (needed2 < 0) {
            free(result);
            return NULL;
        }
        size_t len2 = (size_t)needed2;
        if (current_len + len2 + 1 > capacity) {
            size_t new_cap = capacity * 2;
            if (new_cap < current_len + len2 + 1) {
                new_cap = current_len + len2 + 1;
            }
            char *new_res = (char*)realloc(result, new_cap);
            if (new_res == NULL) {
                free(result);
                return NULL;
            }
            result = new_res;
            capacity = new_cap;
        }
        int written2 = snprintf(result + current_len, len2 + 1, "%d", current->second);
        if (written2 < 0 || (size_t)written2 != len2) {
            free(result);
            return NULL;
        }
        current_len += len2;

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

    char *flat = flatten_tuple_list(&t1);
    if (flat == NULL) {
        printf("Error: failed to flatten list\n");
        return 1;
    }
    printf("%s\n", flat);
    free(flat);

    tuple_t single = {5, 6, NULL};
    char *flat_single = flatten_tuple_list(&single);
    if (flat_single == NULL) {
        printf("Error: failed to flatten single\n");
        return 1;
    }
    printf("%s\n", flat_single);
    free(flat_single);

    char *flat_empty = flatten_tuple_list(NULL);
    if (flat_empty == NULL) {
        printf("Error: failed to flatten empty\n");
        return 1;
    }
    printf("%s\n", flat_empty);
    free(flat_empty);

    return 0;
}