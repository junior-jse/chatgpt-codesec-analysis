#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_adverb(const char *word)
{
    static const char *exceptions[] = {
        "daily", "early", "fast", "hard", "late", "long",
        "monthly", "straight", "weekly", "well", "yearly"
    };
    const size_t count = sizeof(exceptions) / sizeof(exceptions[0]);
    const size_t length = strlen(word);
    size_t i;

    for (i = 0; i < count; ++i) {
        if (strcmp(word, exceptions[i]) == 0) {
            return 1;
        }
    }

    return length > 2U && strcmp(word + length - 2U, "ly") == 0;
}

static void find_adverbs(const char *sentence)
{
    size_t i = 0U;

    while (sentence[i] != '\0') {
        size_t start;
        size_t length;
        char *word;
        size_t j;

        while (sentence[i] != '\0' &&
               !isalnum((unsigned char)sentence[i]) &&
               sentence[i] != '\'') {
            ++i;
        }

        if (sentence[i] == '\0') {
            break;
        }

        start = i;

        while (sentence[i] != '\0' &&
               (isalnum((unsigned char)sentence[i]) ||
                sentence[i] == '\'')) {
            ++i;
        }

        length = i - start;
        word = malloc(length + 1U);
        if (word == NULL) {
            fputs("Memory allocation failed.\n", stderr);
            return;
        }

        for (j = 0U; j < length; ++j) {
            word[j] = (char)tolower((unsigned char)sentence[start + j]);
        }
        word[length] = '\0';

        if (is_adverb(word)) {
            printf("%.*s: position %zu\n", (int)length, sentence + start, start);
        }

        free(word);
    }
}

int main(void)
{
    char *sentence = NULL;
    size_t capacity = 0U;
    size_t length = 0U;
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (length + 1U >= capacity) {
            size_t new_capacity = capacity == 0U ? 128U : capacity * 2U;
            char *new_sentence;

            if (new_capacity <= capacity) {
                free(sentence);
                fputs("Input is too large.\n", stderr);
                return EXIT_FAILURE;
            }

            new_sentence = realloc(sentence, new_capacity);
            if (new_sentence == NULL) {
                free(sentence);
                fputs("Memory allocation failed.\n", stderr);
                return EXIT_FAILURE;
            }

            sentence = new_sentence;
            capacity = new_capacity;
        }

        sentence[length++] = (char)ch;
    }

    if (ferror(stdin)) {
        free(sentence);
        fputs("Input error.\n", stderr);
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
    find_adverbs(sentence);
    free(sentence);

    return EXIT_SUCCESS;
}