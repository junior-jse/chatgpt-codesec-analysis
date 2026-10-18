#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} WordList;

static void free_word_list(WordList *list)
{
    size_t i;

    if (list == NULL) {
        return;
    }

    for (i = 0; i < list->count; ++i) {
        free(list->items[i]);
    }

    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

static int word_equals(const char *stored, const char *word, size_t length)
{
    return strlen(stored) == length && memcmp(stored, word, length) == 0;
}

static int add_word(WordList *list, const char *word, size_t length)
{
    char *copy;

    if (list->count == list->capacity) {
        size_t new_capacity;
        char **new_items;

        if (list->capacity == 0) {
            new_capacity = 8;
        } else {
            if (list->capacity > SIZE_MAX / 2) {
                return 0;
            }
            new_capacity = list->capacity * 2;
        }

        if (new_capacity > SIZE_MAX / sizeof(*new_items)) {
            return 0;
        }

        new_items = realloc(list->items, new_capacity * sizeof(*new_items));
        if (new_items == NULL) {
            return 0;
        }

        list->items = new_items;
        list->capacity = new_capacity;
    }

    if (length == SIZE_MAX) {
        return 0;
    }

    copy = malloc(length + 1);
    if (copy == NULL) {
        return 0;
    }

    memcpy(copy, word, length);
    copy[length] = '\0';
    list->items[list->count++] = copy;

    return 1;
}

static char *find_first_repeated_word(const char *text)
{
    WordList words = {NULL, 0, 0};
    const char *cursor;

    if (text == NULL) {
        return NULL;
    }

    cursor = text;

    while (*cursor != '\0') {
        const char *start;
        size_t length;
        size_t i;

        while (*cursor != '\0' && !isalnum((unsigned char)*cursor)) {
            ++cursor;
        }

        start = cursor;

        while (*cursor != '\0' && isalnum((unsigned char)*cursor)) {
            ++cursor;
        }

        length = (size_t)(cursor - start);
        if (length == 0) {
            continue;
        }

        for (i = 0; i < words.count; ++i) {
            if (word_equals(words.items[i], start, length)) {
                char *result = malloc(length + 1);

                if (result != NULL) {
                    memcpy(result, start, length);
                    result[length] = '\0';
                }

                free_word_list(&words);
                return result;
            }
        }

        if (!add_word(&words, start, length)) {
            free_word_list(&words);
            return NULL;
        }
    }

    free_word_list(&words);
    return NULL;
}

int main(void)
{
    char input[4096];
    char *repeated;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        if (ferror(stdin)) {
            fprintf(stderr, "Failed to read input.\n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    repeated = find_first_repeated_word(input);
    if (repeated != NULL) {
        printf("%s\n", repeated);
        free(repeated);
    } else {
        printf("No repeated word found.\n");
    }

    return EXIT_SUCCESS;
}