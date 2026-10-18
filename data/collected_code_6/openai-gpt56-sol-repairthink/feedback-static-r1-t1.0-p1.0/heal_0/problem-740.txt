#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int key;
    int value;
} KeyValue;

int tuple_to_dictionary(const int *tuple, size_t tuple_size,
                        KeyValue **dictionary, size_t *dictionary_size)
{
    KeyValue *result;
    size_t pair_count;
    size_t i;

    if (tuple == NULL || dictionary == NULL || dictionary_size == NULL) {
        return -1;
    }

    *dictionary = NULL;
    *dictionary_size = 0;

    if (tuple_size % 2 != 0) {
        return -1;
    }

    pair_count = tuple_size / 2;

    if (pair_count == 0) {
        return 0;
    }

    if (pair_count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(pair_count * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    for (i = 0; i < pair_count; ++i) {
        result[i].key = tuple[i * 2];
        result[i].value = tuple[i * 2 + 1];
    }

    *dictionary = result;
    *dictionary_size = pair_count;
    return 0;
}

int main(void)
{
    const int tuple[] = {1, 10, 2, 20, 3, 30};
    const size_t tuple_size = sizeof(tuple) / sizeof(tuple[0]);
    KeyValue *dictionary = NULL;
    size_t dictionary_size = 0;
    size_t i;

    if (tuple_to_dictionary(tuple, tuple_size,
                            &dictionary, &dictionary_size) != 0) {
        fputs("Failed to convert tuple\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < dictionary_size; ++i) {
        printf("%d: %d\n", dictionary[i].key, dictionary[i].value);
    }

    free(dictionary);
    return EXIT_SUCCESS;
}