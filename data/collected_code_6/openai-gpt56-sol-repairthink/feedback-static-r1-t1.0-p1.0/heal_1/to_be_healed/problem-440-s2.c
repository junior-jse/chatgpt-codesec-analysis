#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_word_character(unsigned char c)
{
    return isalpha(c) || c == '\'' || c == '-';
}

static int is_adverb(const char *word)
{
    static const char *const common_adverbs[] = {
        "almost", "always", "fast", "hard", "here", "how", "late",
        "never", "not", "now", "often", "quite", "rather", "soon",
        "there", "today", "tomorrow", "too", "very", "well",
        "when", "where", "why", "yesterday"
    };
    const size_t count = sizeof(common_adverbs) / sizeof(common_adverbs[0]);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    const size_t length = strlen(word);
    size_t i;

    if (length > 2U && strcmp(word + length - 2U, "ly") == 0) {
        return 1;
    }

    for (i = 0U; i < count; ++i) {
        if (strcmp(word, common_adverbs[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

static int find_adverbs(const char *sentence)
{
    size_t position = 0U;
    size_t word_number = 0U;
    int found = 0;

    while (sentence[position] != '\0') {
        size_t start;
        size_t length;
        char *word;
        size_t i;

        while (sentence[position] != '\0' &&
               !is_word_character((unsigned char)sentence[position])) {
            ++position;
        }

        if (sentence[position] == '\0') {
            break;
        }

        start = position;
        while (sentence[position] != '\0' &&
               is_word_character((unsigned char)sentence[position])) {
            ++position;
        }

        length = position - start;
        ++word_number;

        if (length == SIZE_MAX) {
            fputs("Word is too long.\n", stderr);
            return -1;
        }

        word = malloc(length + 1U);
        if (word == NULL) {
            fputs("Memory allocation failed.\n", stderr);
            return -1;
        }

        for (i = 0U; i < length; ++i) {
            word[i] = (char)tolower((unsigned char)sentence[start + i]);
        }
        word[length] = '\0';

        if (is_adverb(word)) {
            printf("%s: word %zu, character %zu\n",
                   word, word_number, start + 1U);
            found = 1;
        }

        free(word);
    }

    if (!found) {
        puts("No adverbs found.");
    }

    return 0;
}

int main(void)
{
    char *sentence = NULL;
    size_t capacity = 0U;
    size_t length = 0U;
    int character;
    int result;

    /* Possible weaknesses found:
     * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((character = getchar()) != '\n' && character != EOF) {
        char *resized;

        if (length + 1U >= capacity) {
            size_t new_capacity = capacity == 0U ? 128U : capacity * 2U;

            if (new_capacity <= capacity) {
                fputs("Input is too long.\n", stderr);
                free(sentence);
                return EXIT_FAILURE;
            }

            resized = realloc(sentence, new_capacity);
            if (resized == NULL) {
                fputs("Memory allocation failed.\n", stderr);
                free(sentence);
                return EXIT_FAILURE;
            }

            sentence = resized;
            capacity = new_capacity;
        }

        sentence[length++] = (char)character;
    }

    if (ferror(stdin)) {
        fputs("Failed to read input.\n", stderr);
        free(sentence);
        return EXIT_FAILURE;
    }

    if (sentence == NULL) {
        sentence = malloc(1U);
        if (sentence == NULL) {
            fputs("Memory allocation failed.\n", stderr);
            return EXIT_FAILURE;
        }
    }

    sentence[length] = '\0';
    result = find_adverbs(sentence);
    free(sentence);

    return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}