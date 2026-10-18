#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int shortlist_words(const char *const words[], size_t word_count,
                           size_t min_length, const char ***result,
                           size_t *result_count)
{
    const char **matches = NULL;
    size_t count = 0;

    if (result == NULL || result_count == NULL ||
        (word_count > 0 && words == NULL)) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    for (size_t i = 0; i < word_count; ++i) {
        if (words[i] == NULL) {
            return -1;
        }

        if (strlen(words[i]) > min_length) {
            ++count;
        }
    }

    if (count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*matches)) {
        return -1;
    }

    matches = malloc(count * sizeof(*matches));
    if (matches == NULL) {
        return -1;
    }

    size_t match_index = 0;
    for (size_t i = 0; i < word_count; ++i) {
        if (strlen(words[i]) > min_length) {
            matches[match_index++] = words[i];
        }
    }

    *result = matches;
    *result_count = count;
    return 0;
}

int main(void)
{
    const char *words[] = {
        "apple",
        "fig",
        "banana",
        "pear",
        "watermelon",
        "kiwi"
    };
    const size_t word_count = sizeof(words) / sizeof(words[0]);
    const size_t min_length = 4;
    const char **shortlisted = NULL;
    size_t shortlisted_count = 0;

    if (shortlist_words(words, word_count, min_length,
                        &shortlisted, &shortlisted_count) != 0) {
        fputs("Failed to shortlist words.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < shortlisted_count; ++i) {
        if (printf("%s\n", shortlisted[i]) < 0) {
            free(shortlisted);
            return EXIT_FAILURE;
        }
    }

    free(shortlisted);
    return EXIT_SUCCESS;
}