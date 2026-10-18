#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **elements;
    size_t count;
} Tuple;

char **adjacent_concat(const Tuple *t, size_t *out_count) {
    if (t == NULL || out_count == NULL || t->count == 0 || t->elements == NULL) {
        return NULL;
    }
    *out_count = t->count - 1;
    char **result = malloc(*out_count * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < *out_count; i++) {
        if (t->elements[i] == NULL || t->elements[i + 1] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        size_t len1 = strlen(t->elements[i]);
        size_t len2 = strlen(t->elements[i + 1]);
        result[i] = malloc(len1 + len2 + 1);
        if (result[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        strcpy(result[i], t->elements[i]);
        strcat(result[i], t->elements[i + 1]);
    }
    return result;
}

void free_result(char **result, size_t count) {
    if (result == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(result[i]);
    }
    free(result);
}

int main(void) {
    char *data[] = {"Hello", "World", "Foo", "Bar"};
    Tuple t = {data, 4};
    size_t out_count = 0;
    char **result = adjacent_concat(&t, &out_count);
    if (result == NULL) {
        fprintf(stderr, "Error: adjacent_concat failed\n");
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < out_count; i++) {
        printf("%s\n", result[i]);
    }
    free_result(result, out_count);
    return EXIT_SUCCESS;
}