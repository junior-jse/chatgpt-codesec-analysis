#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int a;
    int b;
} Tuple;

char* flatten_tuple_list(Tuple* list, size_t count) {
    if (list == NULL && count > 0) {
        return NULL;
    }

    size_t max_len = count * 24;
    char* result = (char*)malloc(max_len + 1);
    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';
    size_t current_len = 0;

    for (size_t i = 0; i < count; i++) {
        int written = snprintf(result + current_len, max_len - current_len, "(%d, %d)", list[i].a, list[i].b);
        if (written < 0 || (size_t)written >= max_len - current_len) {
            free(result);
            return NULL;
        }
        current_len += (size_t)written;

        if (i < count - 1) {
            if (current_len + 2 >= max_len) {
                free(result);
                return NULL;
            }
            result[current_len++] = ',';
            result[current_len++] = ' ';
            result[current_len] = '\0';
        }
    }

    return result;
}

int main() {
    Tuple list[] = {{1, 2}, {3, 4}, {5, 6}};
    size_t count = sizeof(list) / sizeof(list[0]);

    char* str = flatten_tuple_list(list, count);
    if (str == NULL) {
        fprintf(stderr, "Failed to flatten tuple list\n");
        return 1;
    }

    printf("%s\n", str);

    free(str);

    return 0;
}