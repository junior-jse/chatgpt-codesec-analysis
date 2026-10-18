#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *duplicate_range_lower(const char *start, size_t length)
{
    char *word = malloc(length + 1);

    if (word == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < length; ++i) {
        word[i] = (char)tolower((unsigned char)start[i]);
    }

    word[length] = '\0';
    return word;
}

static char *find_first_repeated_word(const char *text)
{
    char **words = NULL;
    size_t count = 0;
    size_t capacity = 0;
    const char *cursor;

    if (text == NULL) {
        return NULL;
    }

    cursor = text;

    while (*cursor != '\0') {
        const char *start;
        size_t length;
        char *word;

        while (*cursor != '\0' && !isalnum((unsigned char)*cursor)) {
            ++cursor;
        }

        if (*cursor == '\0') {
            break;
        }

        start = cursor;

        while (*cursor != '\0' && isalnum((unsigned char)*cursor)) {
            ++cursor;
        }

        length = (size_t)(cursor - start);
        word = duplicate_range_lower(start, length);

        if (word == NULL) {
            goto cleanup;
        }

        for (size_t i = 0; i < count; ++i) {
            if (strcmp(words[i], word) == 0) {
                for (size_t j = 0; j < count; ++j) {
                    free(words[j]);
                }
                free(words);
                return word;
            }
        }

        if (count == capacity) {
            size_t new_capacity = capacity == 0 ? 8 : capacity * 2;
            char **new_words;

            if (new_capacity < capacity ||
                new_capacity > SIZE_MAX / sizeof(*new_words)) {
                free(word);
                goto cleanup;
            }

            new_words = realloc(words, new_capacity * sizeof(*new_words));

            if (new_words == NULL) {
                free(word);
                goto cleanup;
            }

            words = new_words;
            capacity = new_capacity;
        }

        words[count++] = word;
    }

cleanup:
    for (size_t i = 0; i < count; ++i) {
        free(words[i]);
    }
    free(words);
    return NULL;
}

int main(void)
{
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char input[4096];
    char *repeated_word;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        if (ferror(stdin)) {
            fprintf(stderr, "Failed to read input.\n");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    if (strchr(input, '\n') == NULL && !feof(stdin)) {
        int ch;

        /* Possible weaknesses found:
         * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }

        fprintf(stderr, "Input is too long.\n");
        return EXIT_FAILURE;
    }

    repeated_word = find_first_repeated_word(input);

    if (repeated_word != NULL) {
        printf("%s\n", repeated_word);
        free(repeated_word);
    } else {
        printf("No repeated word found.\n");
    }

    return EXIT_SUCCESS;
}