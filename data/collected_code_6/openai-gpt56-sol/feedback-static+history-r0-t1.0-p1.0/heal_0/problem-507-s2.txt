#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_removed(const char *word, const char *const remove_words[],
                      size_t remove_count)
{
    size_t i;

    for (i = 0; i < remove_count; ++i) {
        if (strcmp(word, remove_words[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

static size_t remove_words(char *words[], size_t word_count,
                           const char *const remove_list[],
                           size_t remove_count)
{
    size_t read_index;
    size_t write_index = 0;

    for (read_index = 0; read_index < word_count; ++read_index) {
        if (!is_removed(words[read_index], remove_list, remove_count)) {
            words[write_index++] = words[read_index];
        } else {
            free(words[read_index]);
        }
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
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1);
    return copy;
}

int main(void)
{
    const char *initial_words[] = {
        "apple", "banana", "orange", "banana", "grape", "pear"
    };
    const char *remove_list[] = {
        "banana", "pear"
    };
    const size_t initial_count =
        sizeof(initial_words) / sizeof(initial_words[0]);
    const size_t remove_count =
        sizeof(remove_list) / sizeof(remove_list[0]);
    char **words;
    size_t remaining_count;
    size_t i;

    words = calloc(initial_count, sizeof(*words));
    if (words == NULL) {
        fputs("Memory allocation failed.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < initial_count; ++i) {
        words[i] = duplicate_string(initial_words[i]);
        if (words[i] == NULL) {
            while (i > 0) {
                free(words[--i]);
            }
            free(words);
            fputs("Memory allocation failed.\n", stderr);
            return EXIT_FAILURE;
        }
    }

    remaining_count =
        remove_words(words, initial_count, remove_list, remove_count);

    for (i = 0; i < remaining_count; ++i) {
        if (printf("%s%s", words[i],
                   i + 1 < remaining_count ? " " : "\n") < 0) {
            while (i < remaining_count) {
                free(words[i++]);
            }
            free(words);
            return EXIT_FAILURE;
        }
        free(words[i]);
    }

    free(words);
    return EXIT_SUCCESS;
}