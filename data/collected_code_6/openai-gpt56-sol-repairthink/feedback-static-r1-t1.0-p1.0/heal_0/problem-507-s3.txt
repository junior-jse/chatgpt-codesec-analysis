#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void free_word_list(char **words, size_t count)
{
    if (words == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(words[i]);
    }

    free(words);
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

static int should_remove(const char *word,
                         const char *const remove_words[],
                         size_t remove_count)
{
    if (word == NULL || (remove_count > 0 && remove_words == NULL)) {
        return 0;
    }

    for (size_t i = 0; i < remove_count; ++i) {
        if (remove_words[i] != NULL && strcmp(word, remove_words[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

static int remove_words(const char *const input_words[],
                        size_t input_count,
                        const char *const remove_list[],
                        size_t remove_count,
                        char ***result,
                        size_t *result_count)
{
    char **filtered;
    size_t kept = 0;

    if (result == NULL || result_count == NULL ||
        (input_count > 0 && input_words == NULL) ||
        (remove_count > 0 && remove_list == NULL)) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    if (input_count == 0) {
        return 0;
    }

    if (input_count > SIZE_MAX / sizeof(*filtered)) {
        return -1;
    }

    filtered = calloc(input_count, sizeof(*filtered));
    if (filtered == NULL) {
        return -1;
    }

    for (size_t i = 0; i < input_count; ++i) {
        if (input_words[i] == NULL) {
            free_word_list(filtered, kept);
            return -1;
        }

        if (!should_remove(input_words[i], remove_list, remove_count)) {
            filtered[kept] = duplicate_string(input_words[i]);
            if (filtered[kept] == NULL) {
                free_word_list(filtered, kept);
                return -1;
            }
            ++kept;
        }
    }

    if (kept == 0) {
        free(filtered);
        filtered = NULL;
    } else {
        char **resized = realloc(filtered, kept * sizeof(*filtered));
        if (resized != NULL) {
            filtered = resized;
        }
    }

    *result = filtered;
    *result_count = kept;
    return 0;
}

int main(void)
{
    const char *words[] = {
        "apple", "banana", "orange", "banana", "grape", "pear"
    };
    const char *remove_list[] = {
        "banana", "pear"
    };
    char **filtered = NULL;
    size_t filtered_count = 0;
    int status;

    status = remove_words(
        words,
        sizeof(words) / sizeof(words[0]),
        remove_list,
        sizeof(remove_list) / sizeof(remove_list[0]),
        &filtered,
        &filtered_count
    );

    if (status != 0) {
        fputs("Failed to remove words.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < filtered_count; ++i) {
        if (puts(filtered[i]) == EOF) {
            free_word_list(filtered, filtered_count);
            return EXIT_FAILURE;
        }
    }

    free_word_list(filtered, filtered_count);
    return EXIT_SUCCESS;
}