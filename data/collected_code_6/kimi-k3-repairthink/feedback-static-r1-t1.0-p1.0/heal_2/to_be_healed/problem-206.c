#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 4096

typedef struct {
    char **elements;
    size_t count;
} Tuple;

static size_t bounded_strlen(const char *s, size_t max_len) {
    const char *end = memchr(s, '\0', max_len);
    if (end == NULL) {
        return max_len;
    }
    return (size_t)(end - s);
}

char **adjacent_concatenate(const Tuple *input, size_t *result_count) {
    if (input == NULL || input->elements == NULL || result_count == NULL || input->count == 0) {
        return NULL;
    }

    if (input->count == 1) {
        size_t len = bounded_strlen(input->elements[0], MAX_STRING_LENGTH);
        if (len == MAX_STRING_LENGTH) {
            return NULL;
        }
        char **result = malloc(sizeof(char *));
        if (result == NULL) {
            return NULL;
        }
        result[0] = malloc(len + 1);
        if (result[0] == NULL) {
            free(result);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[0], input->elements[0], len + 1);
        *result_count = 1;
        return result;
    }

    char **result = malloc((input->count - 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < input->count - 1; i++) {
        size_t len1 = bounded_strlen(input->elements[i], MAX_STRING_LENGTH);
        size_t len2 = bounded_strlen(input->elements[i + 1], MAX_STRING_LENGTH);
        if (len1 == MAX_STRING_LENGTH || len2 == MAX_STRING_LENGTH) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        result[i] = malloc(len1 + len2 + 1);
        if (result[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[i], input->elements[i], len1);
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[i] + len1, input->elements[i + 1], len2);
        result[i][len1 + len2] = '\0';
    }

    *result_count = input->count - 1;
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
    char *data[] = {"Hello", "World", "Foo", "Bar", "Baz"};
    Tuple input = {data, 5};
    size_t result_count = 0;

    char **result = adjacent_concatenate(&input, &result_count);

    if (result == NULL) {
        fprintf(stderr, "Error: Memory allocation failed or invalid input\n");
        return EXIT_FAILURE;
    }

    printf("Original tuple: ");
    for (size_t i = 0; i < input.count; i++) {
        printf("%s ", input.elements[i]);
    }
    printf("\n");

    printf("Adjacent concatenation: ");
    for (size_t i = 0; i < result_count; i++) {
        printf("%s ", result[i]);
    }
    printf("\n");

    free_result(result, result_count);

    return EXIT_SUCCESS;
}