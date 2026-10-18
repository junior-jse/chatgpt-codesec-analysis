#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int a;
    int b;
} Tuple;

char *flatten_tuple_list(Tuple *list, size_t size) {
    if (list == NULL && size > 0) {
        return NULL;
    }

    size_t initial_size = 4;
    char *result = malloc(initial_size);
    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';
    size_t capacity = initial_size;
    size_t length = 0;

    for (size_t i = 0; i < size; i++) {
        int written;
        int needed;
        char temp[64];

        needed = snprintf(temp, sizeof(temp), "(%d, %d)", list[i].a, list[i].b);
        if (needed < 0) {
            free(result);
            return NULL;
        }

        size_t needed_size = (size_t)needed;
        size_t total_length = length + needed_size;

        if (total_length >= capacity) {
            size_t new_capacity = capacity;
            while (new_capacity <= total_length) {
                if (new_capacity > SIZE_MAX / 2) {
                    free(result);
                    return NULL;
                }
                new_capacity *= 2;
            }

            char *new_result = realloc(result, new_capacity);
            if (new_result == NULL) {
                free(result);
                return NULL;
            }

            result = new_result;
            capacity = new_capacity;
        }

        memcpy(result + length, temp, needed_size + 1);
        length = total_length;
    }

    if (length == 0 && capacity > 0) {
        result[0] = '\0';
    }

    char *final_result = realloc(result, length + 1);
    if (final_result != NULL) {
        return final_result;
    }

    return result;
}

int main(void) {
    Tuple list[] = {{1, 2}, {3, 4}, {5, 6}};
    size_t size = sizeof(list) / sizeof(list[0]);

    char *flattened = flatten_tuple_list(list, size);
    if (flattened == NULL) {
        fprintf(stderr, "Failed to flatten tuple list\n");
        return 1;
    }

    printf("%s\n", flattened);

    free(flattened);
    flattened = NULL;

    return 0;
}