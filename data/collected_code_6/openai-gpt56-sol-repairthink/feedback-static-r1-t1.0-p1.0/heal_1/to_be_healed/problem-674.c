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

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t length = strlen(word);
    char *copy = malloc(length + 1);

    if (copy == NULL) {
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, word, length + 1);
    set->items[set->size++] = copy;
    return 0;
}

char *remove_duplicate_words(const char *input)
{
    if (input == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_length = strlen(input);
    char *result = malloc(input_length + 1);

    if (result == NULL) {
        return NULL;
    }

    StringSet seen = {0};
    size_t input_index = 0;
    size_t output_index = 0;
    int first_word = 1;

    while (input[input_index] != '\0') {
        while (isspace((unsigned char)input[input_index])) {
            ++input_index;
        }

        if (input[input_index] == '\0') {
            break;
        }

        size_t start = input_index;

        while (input[input_index] != '\0' &&
               !isspace((unsigned char)input[input_index])) {
            ++input_index;
        }

        size_t word_length = input_index - start;
        char *word = malloc(word_length + 1);

        if (word == NULL) {
            string_set_destroy(&seen);
            free(result);
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(word, input + start, word_length);
        word[word_length] = '\0';

        if (!string_set_contains(&seen, word)) {
            if (string_set_add(&seen, word) != 0) {
                free(word);
                string_set_destroy(&seen);
                free(result);
                return NULL;
            }

            if (!first_word) {
                result[output_index++] = ' ';
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + output_index, word, word_length);
            output_index += word_length;
            first_word = 0;
        }

        free(word);
    }

    result[output_index] = '\0';
    string_set_destroy(&seen);
    return result;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[4096];

    if (fgets(input, sizeof(input), stdin) == NULL) {
        if (ferror(stdin)) {
            fprintf(stderr, "Failed to read input.\n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    if (strchr(input, '\n') == NULL && !feof(stdin)) {
        fprintf(stderr, "Input is too long.\n");
        return EXIT_FAILURE;
    }

    input[strcspn(input, "\r\n")] = '\0';

    char *result = remove_duplicate_words(input);

    if (result == NULL) {
        fprintf(stderr, "Failed to process input.\n");
        return EXIT_FAILURE;
    }

    if (printf("%s\n", result) < 0) {
        free(result);
        return EXIT_FAILURE;
    }

    free(result);
    return EXIT_SUCCESS;
}