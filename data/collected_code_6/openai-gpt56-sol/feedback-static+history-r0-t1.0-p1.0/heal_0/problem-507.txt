#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_word_to_remove(const char *word,
                             const char *const words_to_remove[],
                             size_t remove_count)
{
    size_t i;

    for (i = 0; i < remove_count; ++i) {
        if (words_to_remove[i] != NULL &&
            strcmp(word, words_to_remove[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

static size_t remove_words(char *words[],
                           size_t word_count,
                           const char *const words_to_remove[],
                           size_t remove_count)
{
    size_t read_index;
    size_t write_index = 0;

    if (words == NULL || (remove_count > 0 && words_to_remove == NULL)) {
        return word_count;
    }

    for (read_index = 0; read_index < word_count; ++read_index) {
        if (words[read_index] == NULL) {
            continue;
        }

        if (is_word_to_remove(words[read_index],
                              words_to_remove,
                              remove_count)) {
            free(words[read_index]);
        } else {
            words[write_index++] = words[read_index];
        }
    }

    for (read_index = write_index; read_index < word_count; ++read_index) {
        words[read_index] = NULL;
    }

    return write_index;
}

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source);
    copy = malloc(length + 1);

    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1);
    return copy;
}

static void free_word_list(char *words[], size_t word_count)
{
    size_t i;

    if (words == NULL) {
        return;
    }

    for (i = 0; i < word_count; ++i) {
        free(words[i]);
        words[i] = NULL;
    }
}

int main(void)
{
    const char *initial_words[] = {
        "apple", "banana", "orange", "banana", "grape", "pear"
    };
    const char *words_to_remove[] = {
        "banana", "pear"
    };
    enum { WORD_COUNT = sizeof(initial_words) / sizeof(initial_words[0]) };
    char *words[WORD_COUNT] = { NULL };
    size_t count = WORD_COUNT;
    size_t i;

    for (i = 0; i < count; ++i) {
        words[i] = duplicate_string(initial_words[i]);
        if (words[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory.\n");
            free_word_list(words, i);
            return EXIT_FAILURE;
        }
    }

    count = remove_words(
        words,
        count,
        words_to_remove,
        sizeof(words_to_remove) / sizeof(words_to_remove[0])
    );

    for (i = 0; i < count; ++i) {
        if (printf("%s\n", words[i]) < 0) {
            fprintf(stderr, "Failed to write output.\n");
            free_word_list(words, count);
            return EXIT_FAILURE;
        }
    }

    free_word_list(words, count);
    return EXIT_SUCCESS;
}