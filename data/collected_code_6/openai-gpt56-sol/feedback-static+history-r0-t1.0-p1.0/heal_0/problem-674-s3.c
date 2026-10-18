#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t size;
    size_t capacity;
} StringSet;

static void string_set_destroy(StringSet *set)
{
    if (set == NULL) {
        return;
    }

    for (size_t i = 0; i < set->size; ++i) {
        free(set->items[i]);
    }

    free(set->items);
    set->items = NULL;
    set->size = 0;
    set->capacity = 0;
}

static int string_set_contains(const StringSet *set, const char *word)
{
    if (set == NULL || word == NULL) {
        return 0;
    }

    for (size_t i = 0; i < set->size; ++i) {
        if (strcmp(set->items[i], word) == 0) {
            return 1;
        }
    }

    return 0;
}

static int string_set_add(StringSet *set, const char *word)
{
    if (set == NULL || word == NULL) {
        return -1;
    }

    if (string_set_contains(set, word)) {
        return 0;
    }

    if (set->size == set->capacity) {
        size_t new_capacity = set->capacity == 0 ? 8 : set->capacity * 2;

        if (new_capacity < set->capacity ||
            new_capacity > SIZE_MAX / sizeof(*set->items)) {
            return -1;
        }

        char **new_items = realloc(
            set->items, new_capacity * sizeof(*set->items)
        );

        if (new_items == NULL) {
            return -1;
        }

        set->items = new_items;
        set->capacity = new_capacity;
    }

    size_t length = strlen(word);

    if (length == SIZE_MAX) {
        return -1;
    }

    char *copy = malloc(length + 1);

    if (copy == NULL) {
        return -1;
    }

    memcpy(copy, word, length + 1);
    set->items[set->size++] = copy;
    return 1;
}

static int append_word(
    char **output,
    size_t *length,
    size_t *capacity,
    const char *word
)
{
    if (output == NULL || length == NULL || capacity == NULL || word == NULL) {
        return -1;
    }

    size_t word_length = strlen(word);
    size_t separator_length = *length == 0 ? 0 : 1;

    if (word_length > SIZE_MAX - *length - separator_length - 1) {
        return -1;
    }

    size_t required = *length + separator_length + word_length + 1;

    if (required > *capacity) {
        size_t new_capacity = *capacity == 0 ? 16 : *capacity;

        while (new_capacity < required) {
            if (new_capacity > SIZE_MAX / 2) {
                new_capacity = required;
                break;
            }

            new_capacity *= 2;
        }

        char *new_output = realloc(*output, new_capacity);

        if (new_output == NULL) {
            return -1;
        }

        *output = new_output;
        *capacity = new_capacity;
    }

    if (separator_length != 0) {
        (*output)[(*length)++] = ' ';
    }

    memcpy(*output + *length, word, word_length);
    *length += word_length;
    (*output)[*length] = '\0';
    return 0;
}

char *remove_duplicate_words(const char *input)
{
    if (input == NULL) {
        return NULL;
    }

    StringSet words = {0};
    char *output = NULL;
    size_t output_length = 0;
    size_t output_capacity = 0;
    const char *cursor = input;

    while (*cursor != '\0') {
        while (*cursor != '\0' && isspace((unsigned char)*cursor)) {
            ++cursor;
        }

        const char *start = cursor;

        while (*cursor != '\0' && !isspace((unsigned char)*cursor)) {
            ++cursor;
        }

        size_t word_length = (size_t)(cursor - start);

        if (word_length == 0) {
            continue;
        }

        if (word_length == SIZE_MAX) {
            string_set_destroy(&words);
            free(output);
            return NULL;
        }

        char *word = malloc(word_length + 1);

        if (word == NULL) {
            string_set_destroy(&words);
            free(output);
            return NULL;
        }

        memcpy(word, start, word_length);
        word[word_length] = '\0';

        int add_result = string_set_add(&words, word);

        if (add_result < 0 ||
            (add_result > 0 &&
             append_word(
                 &output,
                 &output_length,
                 &output_capacity,
                 word
             ) != 0)) {
            free(word);
            string_set_destroy(&words);
            free(output);
            return NULL;
        }

        free(word);
    }

    string_set_destroy(&words);

    if (output == NULL) {
        output = malloc(1);

        if (output == NULL) {
            return NULL;
        }

        output[0] = '\0';
    }

    return output;
}

int main(void)
{
    char input[4096];

    if (fgets(input, sizeof(input), stdin) == NULL) {
        if (ferror(stdin)) {
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    size_t length = strlen(input);

    if (length > 0 && input[length - 1] == '\n') {
        input[length - 1] = '\0';
    } else if (!feof(stdin)) {
        int character;

        do {
            character = getchar();
        } while (character != '\n' && character != EOF);

        if (ferror(stdin)) {
            return EXIT_FAILURE;
        }

        return EXIT_FAILURE;
    }

    char *result = remove_duplicate_words(input);

    if (result == NULL) {
        return EXIT_FAILURE;
    }

    if (puts(result) == EOF) {
        free(result);
        return EXIT_FAILURE;
    }

    free(result);
    return EXIT_SUCCESS;
}