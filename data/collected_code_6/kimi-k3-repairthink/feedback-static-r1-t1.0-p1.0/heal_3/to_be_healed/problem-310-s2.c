#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 65536

typedef struct {
    char **items;
    size_t count;
} Tuple;

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static int safe_memcpy(void *dest, size_t dest_size, const void *src, size_t count) {
    if (dest == NULL || src == NULL) {
        return -1;
    }
    if (count > dest_size) {
        return -1;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dest, src, count);
    return 0;
}

static void free_tuple(Tuple *tuple) {
    if (tuple == NULL) {
        return;
    }
    if (tuple->items != NULL) {
        for (size_t i = 0; i < tuple->count; i++) {
            free(tuple->items[i]);
        }
        free(tuple->items);
    }
    tuple->items = NULL;
    tuple->count = 0;
}

static char *trim_whitespace(char *str, size_t max_len) {
    size_t len;
    char *end;

    while (*str != '\0' && isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == '\0') {
        return str;
    }

    len = bounded_strlen(str, max_len);
    if (len == 0) {
        return str;
    }

    end = str + len - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    end[1] = '\0';

    return str;
}

int string_to_tuple(const char *input, Tuple *result) {
    char *buffer;
    char *token;
    char *saveptr;
    char **temp_items;
    size_t capacity = 10;
    size_t count = 0;
    size_t input_len;
    size_t buffer_size;

    if (input == NULL || result == NULL) {
        return -1;
    }

    result->items = NULL;
    result->count = 0;

    input_len = bounded_strlen(input, MAX_INPUT_LENGTH);
    if (input_len >= MAX_INPUT_LENGTH) {
        return -1;
    }

    buffer_size = input_len + 1;
    buffer = (char *)malloc(buffer_size);
    if (buffer == NULL) {
        return -1;
    }
    if (safe_memcpy(buffer, buffer_size, input, input_len) != 0) {
        free(buffer);
        return -1;
    }
    buffer[input_len] = '\0';

    temp_items = (char **)malloc(capacity * sizeof(char *));
    if (temp_items == NULL) {
        free(buffer);
        return -1;
    }

    token = strtok_r(buffer, ",", &saveptr);
    while (token != NULL) {
        /* Possible weaknesses found:
         *  Variable 'trimmed' can be declared as pointer to const [constVariablePointer]
         */
        char *trimmed = trim_whitespace(token, input_len);
        size_t trimmed_len = bounded_strlen(trimmed, input_len);
        size_t item_size = trimmed_len + 1;

        if (count >= capacity) {
            char **new_items;
            size_t new_capacity;
            if (capacity > SIZE_MAX / (2 * sizeof(char *))) {
                for (size_t i = 0; i < count; i++) {
                    free(temp_items[i]);
                }
                free(temp_items);
                free(buffer);
                return -1;
            }
            new_capacity = capacity * 2;
            new_items = (char **)realloc(temp_items, new_capacity * sizeof(char *));
            if (new_items == NULL) {
                for (size_t i = 0; i < count; i++) {
                    free(temp_items[i]);
                }
                free(temp_items);
                free(buffer);
                return -1;
            }
            temp_items = new_items;
            capacity = new_capacity;
        }

        temp_items[count] = (char *)malloc(item_size);
        if (temp_items[count] == NULL) {
            for (size_t i = 0; i < count; i++) {
                free(temp_items[i]);
            }
            free(temp_items);
            free(buffer);
            return -1;
        }
        if (safe_memcpy(temp_items[count], item_size, trimmed, trimmed_len) != 0) {
            free(temp_items[count]);
            for (size_t i = 0; i < count; i++) {
                free(temp_items[i]);
            }
            free(temp_items);
            free(buffer);
            return -1;
        }
        temp_items[count][trimmed_len] = '\0';
        count++;

        token = strtok_r(NULL, ",", &saveptr);
    }

    free(buffer);

    if (count == 0) {
        free(temp_items);
        result->items = NULL;
        result->count = 0;
        return 0;
    }

    result->items = temp_items;
    result->count = count;
    return 0;
}

int main(void) {
    const char *input = "apple, banana, cherry, date";
    Tuple tuple;
    int status;

    status = string_to_tuple(input, &tuple);
    if (status != 0) {
        fprintf(stderr, "Error converting string to tuple\n");
        return EXIT_FAILURE;
    }

    printf("(");
    for (size_t i = 0; i < tuple.count; i++) {
        printf("\"%s\"", tuple.items[i]);
        if (i < tuple.count - 1) {
            printf(", ");
        }
    }
    printf(")\n");

    free_tuple(&tuple);

    return EXIT_SUCCESS;
}