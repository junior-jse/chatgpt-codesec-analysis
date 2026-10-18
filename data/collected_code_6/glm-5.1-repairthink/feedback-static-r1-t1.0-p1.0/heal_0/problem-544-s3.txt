#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int first;
    char *second;
} Tuple;

char *flatten_tuple_list(Tuple *list, size_t size) {
    if (list == NULL || size == 0) {
        char *empty = malloc(1);
        if (empty == NULL) return NULL;
        empty[0] = '\0';
        return empty;
    }

    size_t total_length = 1;
    for (size_t i = 0; i < size; i++) {
        total_length += snprintf(NULL, 0, "(%d, %s)", list[i].first, list[i].second ? list[i].second : "null");
        if (i < size - 1) {
            total_length += 2;
        }
    }

    char *result = malloc(total_length);
    if (result == NULL) return NULL;

    char *current = result;
    for (size_t i = 0; i < size; i++) {
        int written = snprintf(current, total_length - (current - result), "(%d, %s)", list[i].first, list[i].second ? list[i].second : "null");
        if (written < 0) {
            free(result);
            return NULL;
        }
        current += written;
        if (i < size - 1) {
            *current++ = ',';
            *current++ = ' ';
        }
    }
    *current = '\0';

    return result;
}

int main(void) {
    Tuple list[] = {
        {1, "apple"},
        {2, "banana"},
        {3, "cherry"}
    };
    size_t size = sizeof(list) / sizeof(list[0]);

    char *flattened = flatten_tuple_list(list, size);
    if (flattened == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    printf("%s\n", flattened);

    free(flattened);
    return 0;
}