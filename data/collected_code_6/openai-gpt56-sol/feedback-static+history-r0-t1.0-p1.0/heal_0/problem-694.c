#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *key;
    const char *value;
} DictionaryEntry;

static int contains_value(const char *const *values, size_t count, const char *value)
{
    size_t i;

    for (i = 0; i < count; ++i) {
        if (strcmp(values[i], value) == 0) {
            return 1;
        }
    }

    return 0;
}

static int extract_unique_values(const DictionaryEntry *dictionary,
                                 size_t dictionary_size,
                                 const char ***unique_values,
                                 size_t *unique_count)
{
    const char **result;
    size_t count;
    size_t i;

    if (unique_values == NULL || unique_count == NULL) {
        return -1;
    }

    *unique_values = NULL;
    *unique_count = 0;

    if (dictionary_size == 0) {
        return 0;
    }

    if (dictionary == NULL ||
        dictionary_size > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(dictionary_size * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    count = 0;

    for (i = 0; i < dictionary_size; ++i) {
        if (dictionary[i].value == NULL) {
            free(result);
            return -1;
        }

        if (!contains_value(result, count, dictionary[i].value)) {
            result[count++] = dictionary[i].value;
        }
    }

    *unique_values = result;
    *unique_count = count;
    return 0;
}

int main(void)
{
    const DictionaryEntry dictionary[] = {
        {"first", "apple"},
        {"second", "banana"},
        {"third", "apple"},
        {"fourth", "orange"},
        {"fifth", "banana"}
    };
    const char **unique_values;
    size_t unique_count;
    size_t i;

    if (extract_unique_values(dictionary,
                              sizeof(dictionary) / sizeof(dictionary[0]),
                              &unique_values,
                              &unique_count) != 0) {
        fputs("Failed to extract unique values.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < unique_count; ++i) {
        if (printf("%s\n", unique_values[i]) < 0) {
            free(unique_values);
            return EXIT_FAILURE;
        }
    }

    free(unique_values);
    return EXIT_SUCCESS;
}