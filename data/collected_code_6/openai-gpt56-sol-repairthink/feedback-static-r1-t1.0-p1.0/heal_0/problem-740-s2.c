#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} KeyValue;

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source);
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1U);
    return copy;
}

static void free_dictionary(KeyValue *dictionary, size_t count)
{
    size_t index;

    if (dictionary == NULL) {
        return;
    }

    for (index = 0U; index < count; ++index) {
        free(dictionary[index].key);
        free(dictionary[index].value);
    }

    free(dictionary);
}

static int tuple_to_dictionary(const char *const tuple[],
                               size_t tuple_count,
                               KeyValue **dictionary,
                               size_t *dictionary_count)
{
    KeyValue *result;
    size_t pair_count;
    size_t index;

    if (tuple == NULL || dictionary == NULL || dictionary_count == NULL ||
        tuple_count == 0U || tuple_count % 2U != 0U) {
        return -1;
    }

    pair_count = tuple_count / 2U;
    if (pair_count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = calloc(pair_count, sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    for (index = 0U; index < pair_count; ++index) {
        result[index].key = duplicate_string(tuple[index * 2U]);
        result[index].value = duplicate_string(tuple[index * 2U + 1U]);

        if (result[index].key == NULL || result[index].value == NULL) {
            free_dictionary(result, pair_count);
            return -1;
        }
    }

    *dictionary = result;
    *dictionary_count = pair_count;
    return 0;
}

int main(void)
{
    const char *tuple[] = {
        "name", "Alice",
        "age", "30",
        "city", "London"
    };
    KeyValue *dictionary = NULL;
    size_t dictionary_count = 0U;
    size_t index;

    if (tuple_to_dictionary(tuple,
                            sizeof(tuple) / sizeof(tuple[0]),
                            &dictionary,
                            &dictionary_count) != 0) {
        fputs("Failed to convert tuple\n", stderr);
        return EXIT_FAILURE;
    }

    for (index = 0U; index < dictionary_count; ++index) {
        if (printf("%s: %s\n",
                   dictionary[index].key,
                   dictionary[index].value) < 0) {
            free_dictionary(dictionary, dictionary_count);
            return EXIT_FAILURE;
        }
    }

    free_dictionary(dictionary, dictionary_count);
    return EXIT_SUCCESS;
}