#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int ends_with_ly(const char *word)
{
    size_t length = strlen(word);

    return length > 2 &&
           tolower((unsigned char)word[length - 2]) == 'l' &&
           tolower((unsigned char)word[length - 1]) == 'y';
}

static void find_adverbs(const char *sentence)
{
    size_t i = 0;
    size_t word_position = 0;

    while (sentence[i] != '\0') {
        size_t start;
        size_t length;
        char *word;

        while (sentence[i] != '\0' &&
               !isalpha((unsigned char)sentence[i])) {
            ++i;
        }

        if (sentence[i] == '\0') {
            break;
        }

        start = i;

        while (sentence[i] != '\0' &&
               (isalpha((unsigned char)sentence[i]) ||
                sentence[i] == '\'' ||
                sentence[i] == '-')) {
            ++i;
        }

        length = i - start;
        ++word_position;

        word = malloc(length + 1);
        if (word == NULL) {
            fputs("Memory allocation failed.\n", stderr);
            return;
        }

        memcpy(word, sentence + start, length);
        word[length] = '\0';

        if (ends_with_ly(word)) {
            printf("%s: word %zu, character %zu\n",
                   word, word_position, start + 1);
        }

        free(word);
    }
}

int main(void)
{
    char *sentence = NULL;
    size_t capacity = 0;
    size_t length = 0;
    int character;
    char *resized;

    fputs("Enter a sentence: ", stdout);
    if (fflush(stdout) == EOF) {
        fputs("Output error.\n", stderr);
        return EXIT_FAILURE;
    }

    while ((character = fgetc(stdin)) != '\n' && character != EOF) {
        if (length + 1 >= capacity) {
            size_t new_capacity;

            if (capacity == 0) {
                new_capacity = 128;
            } else {
                if (capacity > (size_t)-1 / 2) {
                    fputs("Input is too large.\n", stderr);
                    free(sentence);
                    return EXIT_FAILURE;
                }
                new_capacity = capacity * 2;
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
        fputs("Input error.\n", stderr);
        free(sentence);
        return EXIT_FAILURE;
    }

    if (sentence == NULL) {
        sentence = malloc(1);
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